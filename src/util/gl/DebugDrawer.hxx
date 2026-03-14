#pragma once

import lglmodule;

#include <GLEW/glew.h>

#include <gl/VertexArray.hxx>
#include <gl/GpuProgram.hxx>

namespace lgl {
	class DebugDrawer {
	private:
		inline static utl::sptr<GpuProgram> m_program = utl::makeSptr<GpuProgram>();
		inline static utl::vec<glm::vec3> m_vertexData = utl::vec<glm::vec3>();
		inline static utl::uint m_mode = GL_LINES;
		inline static utl::uint m_overrideZ = 0;

	public:
		static void initProgram();
		static void setMode(utl::uint mode) { m_mode = mode; }
		static void setOverrideZ(utl::uint overrideZ) { m_overrideZ = overrideZ; }
		static void setVertexData(const std::vector<glm::vec3>& vertexData);
		static void draw(glm::mat4 V, glm::mat4 P, glm::vec3 color);
	};
}