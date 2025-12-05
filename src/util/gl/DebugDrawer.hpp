#pragma once

#include <GLEW/glew.h>

#include <gl/BaseDefs.hpp>
#include <gl/VertexArray.hpp>
#include <gl/GpuProgram.hpp>

#include <math/Transformation.hpp>
#include <math/Camera.hpp>

namespace lgl {
	class DebugDrawer {
	private:
		static std::shared_ptr<GpuProgram> m_program;
		static std::vector<glm::vec3> m_vertexData;
		static UINT m_mode;
		static UINT m_overrideZ;

	public:
		static void initProgram();
		static void setMode(UINT mode) { m_mode = mode; }
		static void setOverrideZ(UINT overrideZ) { m_overrideZ = overrideZ; }
		static void setVertexData(const std::vector<glm::vec3>& vertexData);
		static void draw(const Camera& camera, glm::vec3 color);
	};
}