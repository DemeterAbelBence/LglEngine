#pragma once 

import lglmodule;
import glmmodule;

#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/SphereMesh.hxx>
#include <collision//SphereCollider.hxx>

namespace lgl {
	class Sphere : public SceneObject {
	private:
		float m_radius;

	public:
		Sphere(float radius);
	};
}

