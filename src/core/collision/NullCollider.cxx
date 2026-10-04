#include "NullCollider.hxx"

namespace lgl {
/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
NullCollider::NullCollider() { geomType = C_NULL; }

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::vector<Collider::ContactData> NullCollider::collidesWith(const CuboidCollider& collidee) const {
  return std::vector<Collider::ContactData>();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::vector<Collider::ContactData> NullCollider::collidesWith(const SphereCollider& collidee) const {
  return std::vector<Collider::ContactData>();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::vector<Collider::ContactData> NullCollider::collidesWith(const TerrainCollider& collidee) const {
  return std::vector<Collider::ContactData>();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::optional<glm::vec3> NullCollider::calculateDepth(const CuboidCollider& collidee,
                                                      const ContactData& contact) const {
  return {};
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::optional<glm::vec3> NullCollider::calculateDepth(const SphereCollider& collidee,
                                                      const ContactData& contact) const {
  return {};
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
std::optional<glm::vec3> NullCollider::calculateDepth(const TerrainCollider& collidee,
                                                      const ContactData& contact) const {
  return {};
}
}  // namespace lgl