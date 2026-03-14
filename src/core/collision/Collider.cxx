#include "Collider.hxx"

#include <collision/TerrainCollider.hxx>
#include <collision/SphereCollider.hxx>

namespace lgl {
    utl::opt<glm::vec3> Collider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::opt<glm::vec3> Collider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::opt<glm::vec3> Collider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::vec<Collider::ContactData> Collider::collidesWith(const Collider& collidee) const {
        if (collidee.geomType == GeomType::C_CUBOID) {
            auto c = dynamic_cast<const CuboidCollider&>(collidee);
            return collidesWith(c);
        }
        if (collidee.geomType == GeomType::C_SPHERE) {
            auto s = dynamic_cast<const SphereCollider&>(collidee);
            return collidesWith(s);
        }
        if (collidee.geomType == GeomType::C_TERRAIN) {
            auto t = dynamic_cast<const TerrainCollider&>(collidee);
            return collidesWith(t);
        }
        return utl::vec<Collider::ContactData>();
    }

    utl::opt<glm::vec3> Collider::calculateContactDepthWith(const Collider& collidee, const ContactData& contact) {
        if (collidee.geomType == C_CUBOID) {
            auto c = dynamic_cast<const CuboidCollider&>(collidee);
            return calculateDepth(c, contact);
        }
        if (collidee.geomType == C_SPHERE) {
            auto s = dynamic_cast<const SphereCollider&>(collidee);
            return calculateDepth(s, contact);
        }
        if (collidee.geomType == C_TERRAIN) {
            auto t = dynamic_cast<const TerrainCollider&>(collidee);
            return calculateDepth(t, contact);
        }
        return {};
    }

    void Collider::updateTransformations() {}
}