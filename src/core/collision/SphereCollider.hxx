#pragma once

import utl;
import glm;
import std;

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
  std::vector<glm::vec3> m_basePoints;
  std::vector<glm::vec3> m_transPoints;

 public:
  SphereCollider(float radius, std::uint32_t n = 100);

  const std::vector<glm::vec3>& getTransPoints() const { return m_transPoints; }
  glm::vec3 getTransCenter() const { return m_transCenter; }

  void updateTransformations() override;

  std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

  std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
};
}  // namespace lgl