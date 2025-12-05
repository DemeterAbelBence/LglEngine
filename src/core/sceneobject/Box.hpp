#pragma once

#include <gl/BaseDefs.hpp>

#include <sceneobject/SceneObject.hpp>

#include <visual/mesh/CuboidMesh.hpp>
#include <visual/mesh/ModelMesh.hpp>

#include <collision/CuboidCollider.hpp>

namespace lgl {
	class LGL_API Box : public SceneObject {
	private:
		glm::vec3 m_dimensions;
		bool m_isStationary;
		const float m_massFactor = 1.0f;

	public:
		Box(bool isStationary, SPTR<ModelMesh> modelMesh);
		Box(const glm::vec3& dimensions, bool isStationary);

		ribo::BodyData initializePhysics() override;
		void stepPhysicsBy(float dt) override;
		void updateRigidBody() override;
		void draw(const Camera& camera) const override;
	};
}