#include "FrameBuffer.hxx"

namespace lgl {
	FrameBuffer::FrameBuffer() : m_FBO(0), m_textureId(0), m_width(0), m_height(0), m_name("") {}

	FrameBuffer::FrameBuffer(utl::cstr name, int width, int height, GLint internalFormat, GLenum type, GLint format) {
		glGenFramebuffers(1, &m_FBO);
		m_name = name;
		m_width = width;
		m_height = height;

		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
			width, height, 0, format, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureId, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Logger::log(Logger::LGL_ERROR, "Framebuffer {} is not complete!\n", name);

		}
		else {
			Logger::log(Logger::LGL_INFO, "Framebuffer {} is complete!\n", name);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_textureId);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::unBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::bindTexture(const GpuProgram& program) const {
		program.bind();
		glActiveTexture(GL_TEXTURE0 + 10);
		program.setInt(m_name, 10);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glActiveTexture(GL_TEXTURE0);
	}
}