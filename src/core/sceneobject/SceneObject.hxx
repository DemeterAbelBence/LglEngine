#pragma once

import lglmodule;
import glmmodule;

#include <gl/FrameBuffer.hxx>
#include <math/ribo.hxx>

#include <visual/mesh/Mesh.hxx>
#include <visual/mesh/ModelMesh.hxx>
#include <visual/common/Light.hxx>
#include <collision/Collider.hxx>
#include <collision/NullCollider.hxx>

namespace lgl {
	class SceneObject {
	protected:
		utl::sptr<Mesh> m_mesh;
		utl::sptr<Collider> m_collider;
		utl::sptr<ribo::PhysicsSolver> m_physicsSolver;
		utl::sptr<Transformation> m_transformation;

	private:
		utl::str m_name;

	public:
		SceneObject();
		SceneObject(float size, utl::sptr<ModelMesh> mesh);

		const utl::sptr<Mesh>& getMesh() const { return m_mesh; }
		const utl::sptr<Collider>& getCollider() const { return m_collider; }
		const utl::sptr<ribo::PhysicsSolver>& getPhysicsSolver() const { return m_physicsSolver; }
		const utl::sptr<Transformation>& getTransformation() const { return m_transformation; }
		const utl::str getName() const { return m_name; }

		void setMesh(utl::sptr<Mesh> mesh) { m_mesh = mesh; }
		void setProgram(utl::sptr<GpuProgram> program);
		void setMaterial(utl::sptr<Material> material);
		void setTextures(const utl::vec<utl::sptr<Texture>>& textures);
		void setName(const utl::str& name) { m_name = name; }

		void translate(glm::vec3 transalte);
		void scale(glm::vec3 scale);
		void setRotation(glm::mat4 R);
		void updateTransformations();

		virtual utl::sptr<SceneObject> clone();
		virtual ribo::BodyData initializePhysics();
		virtual void updateRigidBody();
		virtual void stepPhysicsBy(float dt);
		virtual void draw(const Camera& camera) const;
		virtual void handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L);
	};
}