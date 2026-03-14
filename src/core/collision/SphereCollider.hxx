#pragma once

import lglmodule;
import glmmodule;

#include <collision/Collider.hxx>
#include <collision/CuboidCollider.hxx>
#include <collision/TerrainCollider.hxx>

namespace lgl {
	class SphereCollider : public Collider {
	private:
		glm::vec3 m_transCenter;
		glm::vec3 m_baseCenter;
		float m_radius;

	private:
		utl::vec<glm::vec3> m_basePoints;
		utl::vec<glm::vec3> m_transPoints;

	public:
		SphereCollider(float radius, utl::uint n = 100);

		const utl::vec<glm::vec3>& getTransPoints() const { return m_transPoints; }
		glm::vec3 getTransCenter() const { return m_transCenter; }

		void updateTransformations() override;

		utl::vec<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const SphereCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		utl::opt<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}