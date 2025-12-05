#pragma once

#include <gl/BaseDefs.hpp>

#include <collision/Collider.hpp>
#include <collision/TerrainCollider.hpp>

namespace lgl
{
	class LGL_API CuboidCollider : public Collider {

	public:
		struct Side {
			std::array<glm::vec3, 4> points;
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

			std::array<glm::vec3, 8> points;
			std::array<Side, 6> sides;
			std::vector<glm::vec3> subdivision;
		};

	private:
		CuboidData baseData;
		CuboidData transData;

	private:
		void createCorners();
		void createSides();
		Side createSide(const std::array<UINT, 4>& indices) const;
		void createSubdivision(UINT subdivideInto);

	public:
		static float vertexFaceThreshold;
		static float edgeEdgeThreshold;

	private:
		std::vector<glm::vec3> getEdgesOf(const CuboidCollider& collider) const;
		std::optional<ContactData> calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const;
		std::vector<ContactData> edgeEdgeCollision(const CuboidCollider& collidee) const;

		bool calculateSideIntersection(glm::vec3 point, const Side& side) const;
		std::vector<ContactData> vertexFaceCollision(const CuboidCollider& collidee) const;

	public:
		CuboidCollider(glm::vec3 dimensions);
		CuboidCollider(const CuboidCollider& c);

		const CuboidData& getBaseData() const { return baseData; }
		const CuboidData& getTransData() const { return transData; }

		std::vector<glm::vec3> getSideDrawData(UINT sideIndex);
		glm::vec3 calculateSideCenter(const Side& side) const;

		void updateTransformations() override;

		std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		virtual std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		virtual std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		virtual std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}

