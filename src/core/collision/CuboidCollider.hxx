#pragma once

import lglmodule;
import glmmodule;

#include <collision/Collider.hxx>
#include <collision/SphereCollider.hxx>
#include <collision/TerrainCollider.hxx>

namespace lgl {
	class CuboidCollider : public Collider {
	public:
		struct Side {
			utl::arr<glm::vec3, 4> points;
			glm::vec3 normal;
		};

		struct Edge {
			glm::vec3 point;
			glm::vec3 vector;
		};

		struct CuboidData {
			float width;
			float length;
			float height;

			utl::arr<glm::vec3, 8> points;
			utl::arr<Side, 6> sides;
			utl::vec<glm::vec3> subdivision;
		};

	private:
		CuboidData baseData;
		CuboidData transData;

	private:
		void createCorners();
		void createSides();
		Side createSide(const utl::arr<utl::uint, 4>& indices) const;
		void createSubdivision(utl::uint subdivideInto);

	public:
		inline static float vertexFaceThreshold = 1.0f;
		inline static float edgeEdgeThreshold = 1.0f;

	private:
		utl::vec<glm::vec3> getEdgesOf(const CuboidCollider& collider) const;
		utl::opt<ContactData> calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const;
		utl::vec<ContactData> edgeEdgeCollision(const CuboidCollider& collidee) const;

		bool calculateSideIntersection(glm::vec3 point, const Side& side) const;
		utl::vec<ContactData> vertexFaceCollision(const CuboidCollider& collidee) const;

	public:
		CuboidCollider(glm::vec3 dimensions);
		CuboidCollider(const CuboidCollider& c);
		CuboidCollider(CuboidCollider&& c) noexcept = delete;

		CuboidCollider& operator=(const CuboidCollider& c);

		const CuboidData& getBaseData() const { return baseData; }
		const CuboidData& getTransData() const { return transData; }

		utl::vec<glm::vec3> getSideDrawData(utl::uint sideIndex);
		glm::vec3 calculateSideCenter(const Side& side) const;

		void updateTransformations() override;

		utl::vec<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const SphereCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		virtual utl::opt<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		virtual utl::opt<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		virtual utl::opt<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}

