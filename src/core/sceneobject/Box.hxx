#pragma once

import lglmodule;
import glmmodule;

#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/CuboidMesh.hxx>
#include <visual/mesh/ModelMesh.hxx>
#include <collision/CuboidCollider.hxx>

namespace lgl {
	class Box : public SceneObject {
	private:
		glm::vec3 m_dimensions;
		bool m_isStationary;
		const float m_massFactor = 1.0f;

	public:
		Box();
		Box(bool isStationary, utl::sptr<ModelMesh> modelMesh);
		Box(bool isStationary, const glm::vec3& dimensions);

		utl::sptr<SceneObject> clone() override;
		ribo::BodyData initializePhysics() override;
		void stepPhysicsBy(float dt) override;
		void updateRigidBody() override;
		void draw(const Camera& camera) const override;
	};
}