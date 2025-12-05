#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>
#include <gl/GpuProgram.hpp>

namespace lgl {
	class LGL_API FrameBuffer {
	private:
		UINT fbo;
		UINT textureId;
		int width, height;
		CSTRING name;

	public:
		FrameBuffer() : fbo(0), textureId(0), width(0), height(0), name("") {}
		FrameBuffer(CSTRING _name, int _width, int _height, GLint internalFormat, GLenum type, GLint format);
		~FrameBuffer() { glDeleteFramebuffers(1, &fbo); glDeleteTextures(1, &textureId); }

		void bind() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
		void unBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
		void bindTexture(const GpuProgram& program) const;
	};
}