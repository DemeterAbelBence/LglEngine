#pragma once

import lglmodule;

#include <gl/GpuProgram.hxx>

#include <GLEW/glew.h>

namespace lgl {
	class FrameBuffer {
	private:
		utl::uint m_FBO;
		utl::uint m_textureId;
		int m_width, m_height;
		utl::cstr m_name;

	public:
		FrameBuffer();
		FrameBuffer(utl::cstr name, int width, int height, GLint internalFormat, GLenum type, GLint format);
		~FrameBuffer();

		void bind();
		void unBind();
		void bindTexture(const GpuProgram& program) const;
	};
}