#pragma once 

#include <sceneobject/SceneObject.hpp>

#include <visual/mesh/SphereMesh.hpp>
#include <collision//SphereCollider.hpp>

namespace lgl {
	class LGL_API Sphere : public SceneObject {
	private:
		float m_radius;

	public:
		Sphere(float radius);
		void stepPhysicsBy(float dt) override;
	};
}

