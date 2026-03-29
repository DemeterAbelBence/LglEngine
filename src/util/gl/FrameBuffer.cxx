#include "FrameBuffer.hxx"

namespace lgl {
	FrameBuffer::FrameBuffer() : m_FBO(0), m_RBO(0), m_textureId(0), m_width(0), m_height(0), m_name("") {}

	FrameBuffer::FrameBuffer(utl::cstr name, utl::uint width, utl::uint height, GLint internalFormat, GLenum type, GLint format) {
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

		if (format == GL_DEPTH_COMPONENT) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureId, 0);

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

			glGenRenderbuffers(1, &m_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Logger::log(Logger::LGL_ERROR, "Framebuffer {} is not complete!\n", name);

		}
		else {
			Logger::log(Logger::LGL_INFO, "Framebuffer {} is complete!\n\n", name);
			textureUnitId = textureUnitIdCount++;
			if(textureUnitIdCount >= 16) {
				Logger::log(Logger::LGL_WARN, "Texture unit ID count has reached the maximum limit!\n");
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_textureId);
		glDeleteRenderbuffers(1, &m_RBO);
	}

	int FrameBuffer::getWidth() const {
		return m_width;
	}

	int FrameBuffer::getHeight() const {
		return m_height;
	}

	utl::uint FrameBuffer::getTextureId() const {
		return m_textureId;
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		if (m_RBO == 0) {
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void FrameBuffer::unBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bindTexture(const GpuProgram& program) const {
		program.bind();
		glActiveTexture(GL_TEXTURE0 + textureUnitId);
		program.setInt(m_name, textureUnitId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glActiveTexture(GL_TEXTURE0);
	}
}