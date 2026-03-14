#pragma once

import lglmodule;

#include <collision/Collider.hxx>
#include <collision/CuboidCollider.hxx>

namespace lgl {
	class NullCollider : public Collider {
	public:
		NullCollider();

		utl::vec<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const SphereCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		utl::opt<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}