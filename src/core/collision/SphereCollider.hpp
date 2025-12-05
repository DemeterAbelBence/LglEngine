#pragma once

#include <collision/Collider.hpp>

namespace lgl {
	class LGL_API SphereCollider : public Collider {
	private:
		glm::vec3 transCenter;
		glm::vec3 baseCenter;
		float radius;

	private:
		std::vector<glm::vec3> basePoints;
		std::vector<glm::vec3> transPoints;

	public:
		SphereCollider(float _radius, UINT n = 100);

		const std::vector<glm::vec3>& getTransPoints() const { return transPoints; }
		glm::vec3 getTransCenter() const { return transCenter; }

		void updateTransformations() override;

		std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}