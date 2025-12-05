#pragma once

#include <gl/BaseDefs.hpp>
#include <gl/DebugDrawer.hpp>

#include <sceneobject/SceneObject.hpp>

#include <collision/CuboidCollider.hpp>
#include <collision/TerrainCollider.hpp>
#include <collision/SphereCollider.hpp>
#include <collision/NullCollider.hpp>

namespace lgl {
    class LGL_API CollisionHandler {
    public:
        static bool apply_impulse;
        static bool apply_displacement;
        static bool enableDebug;
        static bool enableInteractions;
        static bool draw_normals;

        static float elasticity;
        static float slipperiness;

    private:
        using CONTACT = std::tuple<SceneObject*, SceneObject*, std::vector<Collider::ContactData>>;
        using VECTOR = std::vector<std::shared_ptr<SceneObject>>;

    private:
        static void debugContact(const CONTACT& interaction, const Camera& camera);
        static void drawCollidersOf(const VECTOR& sceneObjects, const Camera& camera);

        static bool isCollidingContact(float relativeVelocity);
        static std::vector<CONTACT> calculateInteractions(const VECTOR& sceneObjects);
        static void pushObjectsApart(const CONTACT& interaction);
        static void applyImpulse(const CONTACT& interaction);

    public:
        static void handleCollisions(const VECTOR& sceneObjects, const Camera& camera);
    };
}
