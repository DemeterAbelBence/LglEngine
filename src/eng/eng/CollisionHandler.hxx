#pragma once

import lglmodule;
import glmmodule;
import qpmodule;

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
        inline static bool enableBisection = true;
        inline static bool enableImpulses = true;
        inline static bool enableRestingForces = true;
		inline static bool enableDepenetration = false;
		inline static bool pushApartOnce = false;
		inline static bool enableContactLog = true;
        inline static bool enableBisectionLog = true;
		inline static bool enablePhysicsLog = true;
		inline static bool drawNormals = false;
        inline static bool logStatesOnce = false;

        inline static float elasticity = 0.3f;
        inline static float bisectionBias = 0.0001f;
		inline static float contactBias = 0.005f;
		inline static float PSDTolerance = 1e-6f;
        inline static float depthBias = 0.000001f;
        inline static float maxRestingForce = 2143289344.0f / 2.0f;

        enum ContactType {
            COLLIDING,
			SEPARATING,
			RESTING
        };

        using CONTACT = utl::tup<utl::uint, Collider::ContactData, SceneObject*, SceneObject*>;
        inline static utl::vec<CONTACT> currentContacts = utl::vec<CONTACT>();
		inline static utl::vec<CONTACT> restingContacts = utl::vec<CONTACT>();

		inline static bool logContacts = false;
        inline static float bisectedTime = -1.0f;

    private:
        // Calculate contacts
        static void resolveCollisions(utl::vec<CONTACT>& contacts, const utl::svec<SceneObject>& sceneObjects);
        static utl::vec<CONTACT> calculateContactsWithBisection(utl::svec<SceneObject>& sceneObjects);

		// Impulse resolution
        static ContactType getContactType(float relativeVelocity);
        static void applyImpulses();
        static void reclassifyContacts(utl::svec<SceneObject>& sceneObjects);

		// Resting contact resolution
        static void computeRestingContactMatrix(eig::matd& matrix);
        static void computeRestingContactVector(eig::vecd& vector);
		static void resolveRestingContacts();

		// Interpenetration resolution
        static void resolveInterpenetrations(utl::svec<SceneObject>& sceneObjects);

		// Debug and draw
        static void debugContacts(const Camera& camera);
        static void drawCollidersOf(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);  

    public:
		// Main entry point
        static void handleCollisions(utl::svec<SceneObject>& sceneObjects);
        static void debugDrawCollisions(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);
    };
}
