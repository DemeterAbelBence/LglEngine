#pragma once

#include <gl/GpuProgram.hpp>
#include <gl/VertexArray.hpp>

namespace lgl {
	class LGL_API Rect {

	private:
		std::unique_ptr<VertexArray> m_vertexArray;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<VertexBufferLayout> m_bufferLayout;

		std::unique_ptr<GpuProgram> m_shader;

		glm::vec3 m_color;

	public:
		Rect(glm::vec2 corner1, glm::vec2 corner2, glm::vec3 color);
		void draw() const;
	};
}