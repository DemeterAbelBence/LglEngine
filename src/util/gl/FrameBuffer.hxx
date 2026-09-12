#pragma once

import utl;
import std;

#include <gl/GpuProgram.hxx>

#include <GLEW/glew.h>

namespace lgl {
	class FrameBuffer {
	private:
		std::uint32_t m_FBO;
		std::uint32_t m_RBO;

		std::uint32_t m_textureId;
		std::uint32_t m_width, m_height;

		std::string m_name;

		inline static std::uint32_t textureUnitIdCount = 10;
		std::uint32_t textureUnitId;

	public:
		FrameBuffer();
		FrameBuffer(const std::string& name, std::uint32_t width, std::uint32_t height, GLint internalFormat, GLenum type, GLint format);
		~FrameBuffer();
		int getWidth() const;
		int getHeight() const;
		std::uint32_t getTextureId() const;

		void bind();
		void unBind();
		void bindTexture(const GpuProgram& program) const;
	};
}