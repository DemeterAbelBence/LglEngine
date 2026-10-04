#pragma once

import utl;
import glm;

#include <collision/Collider.hxx>
#include <collision/CuboidCollider.hxx>

namespace lgl {
/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class NullCollider : public Collider {
 public:
  NullCollider();

  std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

  std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
};
}  // namespace lgl