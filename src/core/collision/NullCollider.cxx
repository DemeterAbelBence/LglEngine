#include "NullCollider.hxx"

namespace lgl {
    NullCollider::NullCollider() {
        geomType = C_NULL;
    }

    utl::vec<Collider::ContactData> NullCollider::collidesWith(const CuboidCollider& collidee) const {
        return utl::vec<Collider::ContactData>();
    }

    utl::vec<Collider::ContactData> NullCollider::collidesWith(const SphereCollider& collidee) const {
        return utl::vec<Collider::ContactData>();
    }

    utl::vec<Collider::ContactData> NullCollider::collidesWith(const TerrainCollider& collidee) const {
        return utl::vec<Collider::ContactData>();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    utl::opt<glm::vec3> NullCollider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::opt<glm::vec3> NullCollider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::opt<glm::vec3> NullCollider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
        return {};
    }
}