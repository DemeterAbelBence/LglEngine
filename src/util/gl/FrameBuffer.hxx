#pragma once

import lglmodule;

#include <gl/GpuProgram.hxx>

#include <GLEW/glew.h>

namespace lgl {
	class FrameBuffer {
	private:
		utl::uint m_FBO;
		utl::uint m_RBO;

		utl::uint m_textureId;
		utl::uint m_width, m_height;

		utl::cstr m_name;

		inline static utl::uint textureUnitIdCount = 10;
		utl::uint textureUnitId;

	public:
		FrameBuffer();
		FrameBuffer(utl::cstr name, utl::uint width, utl::uint height, GLint internalFormat, GLenum type, GLint format);
		~FrameBuffer();
		int getWidth() const;
		int getHeight() const;
		utl::uint getTextureId() const;

		void bind();
		void unBind();
		void bindTexture(const GpuProgram& program) const;
	};
}