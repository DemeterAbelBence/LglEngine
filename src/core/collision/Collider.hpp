#pragma once

#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>

#include <math/Transformation.hpp>

namespace lgl {
	class CuboidCollider;
	class TerrainCollider;
	class SphereCollider;

	class LGL_API Collider {
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
			std::optional<glm::vec3> depth;
			bool isVertexFace;

			//edge-edge contact data
			std::array<glm::vec3, 2> edgeA;
			std::array<glm::vec3, 2> edgeB;

			//vertex-face contact data
			glm::vec3 facePoint;
		};

	protected:
		Transformation* transformation;

	protected:
		virtual std::vector<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
		virtual std::vector<ContactData> collidesWith(const TerrainCollider& collider) const = 0;
		virtual std::vector<ContactData> collidesWith(const SphereCollider& collider) const = 0;

		virtual std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const;
		virtual std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const;
		virtual std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const;

	public:
		void setTransformation(Transformation* value) { transformation = value; }
		const Transformation* getTransformation() const { return transformation; }
		Collider::GeomType getColliderType() const { return geomType; }

		std::vector<ContactData> collidesWith(const Collider& collidee) const;
		std::optional<glm::vec3> calculateContactDepthWith(const Collider& collidee, const ContactData& contact);

		virtual void updateTransformations();
	};
}