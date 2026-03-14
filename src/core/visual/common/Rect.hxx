#pragma once

import lglmodule;
import glmmodule;

#include <gl/GpuProgram.hxx>
#include <gl/VertexArray.hxx>

namespace lgl {
	class Rect {

	private:
		utl::uptr<VertexArray> m_vertexArray;
		utl::uptr<VertexBuffer> m_vertexBuffer;
		utl::uptr<VertexBufferLayout> m_bufferLayout;

		utl::uptr<GpuProgram> m_shader;

		glm::vec3 m_color;

	public:
		Rect(glm::vec2 corner1, glm::vec2 corner2, glm::vec3 color);
		void draw() const;
	};
}