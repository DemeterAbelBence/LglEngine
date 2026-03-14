#pragma once

import lglmodule;
import glmmodule;

#include <visual/mesh/Surface.hxx>

namespace lgl {
	class SphereMesh : public Surface {
	private:
		float m_radius;

	public:
		SphereMesh(utl::uint N, utl::uint M, float radius);
		void eval(float u, float v, glm::vec3& pos, glm::vec3& norm) override;
	};
}