#include "NullCollider.hpp"

using namespace lgl;

NullCollider::NullCollider() {
    geomType = C_NULL;
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const CuboidCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const SphereCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const TerrainCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::optional<glm::vec3> NullCollider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const {
    return {};
}

std::optional<glm::vec3> NullCollider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
    return {};
}

std::optional<glm::vec3> NullCollider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
    return {};
}
