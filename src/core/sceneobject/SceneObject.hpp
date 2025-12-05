#pragma once

#include <gl/BaseDefs.hpp>
#include <gl/FrameBuffer.hpp>

#include <math/RigidBody.hpp>

#include <visual/mesh/Mesh.hpp>
#include <visual/common/Light.hpp>

#include <collision/Collider.hpp>

namespace lgl {
	class LGL_API SceneObject {
	protected:
		SPTR<Mesh> m_mesh;
		SPTR<Collider> m_collider;
		SPTR<ribo::PhysicsSolver> m_physicsSolver;
		SPTR<Transformation> m_transformation;

	public:
		std::string m_name;

	public:
		SceneObject();

		const SPTR<Mesh>& getMesh() const { return m_mesh; }
		const SPTR<Collider>& getCollider() const { return m_collider; }
		const SPTR<ribo::PhysicsSolver>& getPhysicsSolver() const { return m_physicsSolver; }
		const SPTR<Transformation>& getTransformation() const { return m_transformation; }

		void setMesh(SPTR<Mesh> mesh) { m_mesh = mesh; }
		void setProgram(SPTR<GpuProgram> program);
		void setMaterial(SPTR<Material> material);
		void setTextures(const std::vector<SPTR<Texture>>& textures);

		void translate(glm::vec3 transalte);
		void scale(glm::vec3 scale);
		void setRotation(glm::mat4 R);
		void updateTransformations();

		virtual ribo::BodyData initializePhysics();
		virtual void updateRigidBody();
		virtual void stepPhysicsBy(float dt);
		virtual void resetBodyState();
		virtual void draw(const Camera& camera) const;
		virtual void handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L);
	};
}