#pragma once

#include <visual/mesh/Surface.hpp>

namespace lgl {
	class LGL_API SphereMesh : public Surface {
	private:
		float m_radius;

	public:
		SphereMesh(UINT N, UINT M, float radius);
		void eval(float u, float v, glm::vec3& pos, glm::vec3& norm) override;
	};
}