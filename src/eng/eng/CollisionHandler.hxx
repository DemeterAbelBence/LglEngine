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
        inline static bool enableDebug = true;
		inline static bool enableDisplacement = true;
        inline static bool enableImpulses = true;
        inline static bool enableBisection = true;
        inline static bool drawNormals = false;
		inline static bool hasFriction = true;

        inline static float elasticity = 0.5f;
        inline static float slipperiness = 0.5f;
        inline static float depthBias = 0.0001f;

        enum ContactType {
            COLLIDING,
			SEPARATING,
			RESTING
        };

    private:
        using CONTACT = utl::tup<SceneObject*, SceneObject*, utl::vec<Collider::ContactData>>;

        inline static utl::vec<CONTACT> currentContacts = utl::vec<CONTACT>();

    private:
        static void debugContact(const CONTACT& interaction, const Camera& camera);
        static void drawCollidersOf(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);

        static utl::vec<CONTACT> calculateContacts(utl::svec<SceneObject>& sceneObjects);
        static void pushObjectsApart(const CONTACT& interaction);
        static ContactType getContactType(float relativeVelocity);
        static void applyImpulse(const CONTACT& interaction);

    public:
        static void handleCollisions(utl::svec<SceneObject>& sceneObjects);
        static void debugDrawCollisions(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);
    };
}
