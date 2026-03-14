#pragma once

import lglmodule;
import glmmodule;

#include <math/Transformation.hxx>

namespace lgl {
	class CuboidCollider;
	class TerrainCollider;
	class SphereCollider;

	class Collider {
	public:
		enum GeomType {
			C_CUBOID,
			C_SPHERE,
			C_TERRAIN,
			C_NULL
		};
		GeomType geomType = C_NULL;

	public:
		struct ContactData {
			//base contact data
			glm::vec3 point;
			glm::vec3 normal;
			utl::opt<glm::vec3> depth;
			bool isVertexFace;

			//edge-edge contact data
			utl::arr<glm::vec3, 2> edgeA;
			utl::arr<glm::vec3, 2> edgeB;

			//vertex-face contact data
			glm::vec3 facePoint;
		};

	protected:
		Transformation* transformation;

	protected:
		virtual utl::vec<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
		virtual utl::vec<ContactData> collidesWith(const TerrainCollider& collider) const = 0;
		virtual utl::vec<ContactData> collidesWith(const SphereCollider& collider) const = 0;

		virtual utl::opt<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const;
		virtual utl::opt<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const;
		virtual utl::opt<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const;

	public:
		void setTransformation(Transformation* value) { transformation = value; }
		const Transformation* getTransformation() const { return transformation; }
		Collider::GeomType getColliderType() const { return geomType; }

		utl::vec<ContactData> collidesWith(const Collider& collidee) const;
		utl::opt<glm::vec3> calculateContactDepthWith(const Collider& collidee, const ContactData& contact);

		virtual void updateTransformations();
	};
}