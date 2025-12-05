#pragma once

#include <collision/Collider.hpp>
#include <collision/CuboidCollider.hpp>

namespace lgl {
	class LGL_API NullCollider : public Collider {
	public:
		NullCollider();

		std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
		std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}