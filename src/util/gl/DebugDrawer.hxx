#pragma once

import utl;
import std;

#include <GLEW/glew.h>

#include <gl/VertexArray.hxx>
#include <gl/GpuProgram.hxx>

namespace lgl {
	class DebugDrawer {
	private:
		inline static std::shared_ptr<GpuProgram> m_program = std::make_shared<GpuProgram>();
		inline static std::vector<glm::vec3> m_vertexData = std::vector<glm::vec3>();
		inline static std::uint32_t m_mode = GL_LINES;
		inline static std::uint32_t m_overrideZ = 0;

	public:
		static void initProgram();
		static void setMode(std::uint32_t mode) { m_mode = mode; }
		static void setOverrideZ(std::uint32_t overrideZ) { m_overrideZ = overrideZ; }
		static void setVertexData(const std::vector<glm::vec3>& vertexData);
		static void draw(glm::mat4 V, glm::mat4 P, glm::vec3 color);
	};
}