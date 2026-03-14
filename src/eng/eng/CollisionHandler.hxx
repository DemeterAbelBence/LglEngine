#pragma once

import lglmodule;
import glmmodule;

#include <gl/DebugDrawer.hxx>
#include <sceneobject/SceneObject.hxx>
#include <collision/CuboidCollider.hxx>
#include <collision/TerrainCollider.hxx>
#include <collision/SphereCollider.hxx>
#include <collision/NullCollider.hxx>

namespace lgl {
    class CollisionHandler {
    public:
        inline static bool apply_impulse = false;
        inline static bool apply_displacement = false;
        inline static bool enableDebug = true;
        inline static bool enableInteractions = true;
        inline static bool draw_normals = true;

        inline static float elasticity = 0.5f;
        inline static float slipperiness = 0.5f;

    private:
        using CONTACT = utl::tup<SceneObject*, SceneObject*, utl::vec<Collider::ContactData>>;

    private:
        static void debugContact(const CONTACT& interaction, const Camera& camera);
        static void drawCollidersOf(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);

        static bool isCollidingContact(float relativeVelocity);
        static utl::vec<CONTACT> calculateInteractions(const const utl::svec<SceneObject>& sceneObjects);
        static void pushObjectsApart(const CONTACT& interaction);
        static void applyImpulse(const CONTACT& interaction);

    public:
        static void handleCollisions(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);
    };
}
