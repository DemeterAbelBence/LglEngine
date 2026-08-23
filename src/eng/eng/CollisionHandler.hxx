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
        inline static bool enableRestingForces = false;
		inline static bool enablePushingApart = false;
		inline static bool enableSplitImpulse = false;
		inline static bool pushApartOnce = false;
		inline static bool enableContactLog = false;
        inline static bool enableBisectionLog = false;
		inline static bool drawNormals = false;

        inline static float elasticity = 0.3f;
        inline static float bisectionBias = 0.0001f;
		inline static float contactBias = 0.005f;
		inline static float PSDTolerance = 1e-6f;
        inline static float depthBias = 0.000001f;
        inline static float maxRestingForce = 2143289344.0f / 2.0f;

        inline static int splitImpulseIterations = 5;
        inline static float splitImpulseCorrectionFactor = 0.4f;

        enum ContactType {
            COLLIDING,
			SEPARATING,
			RESTING
        };

    private:
        inline static utl::set<SceneObject*> forzenObjects = utl::set<SceneObject*>();

        using CONTACT = utl::tup<utl::uint, Collider::ContactData, SceneObject*, SceneObject*>;
        inline static utl::vec<CONTACT> currentContacts = utl::vec<CONTACT>();
		inline static utl::vec<CONTACT> restingContacts = utl::vec<CONTACT>();

		inline static bool logContacts = false;
        inline static float bisectedTime = -1.0f;

    private:
        static void resolveCollisions(utl::vec<CONTACT>& contacts, const utl::svec<SceneObject>& sceneObjects);
        static utl::vec<CONTACT> calculateContactsWithBisection(utl::svec<SceneObject>& sceneObjects);

        static void resolveInterpenetrations(utl::svec<SceneObject>& sceneObjects);

        static ContactType getContactType(float relativeVelocity);
        static void applyImpulses();
        static void reclassifyContacts(utl::svec<SceneObject>& sceneObjects);
		static void resolveRestingContacts();

        static void computeRestingContactMatrix(eig::matd& matrix);
        static void computeRestingContactVector(eig::vecd& vector);

        static float calculateMaxDepth(utl::vec<CONTACT>& contacts);

        static void debugContacts(const Camera& camera);
        static void drawCollidersOf(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);

    public:
		static void unfreezeObject(SceneObject* object) { forzenObjects.erase(object); }
		static void freezeObject(SceneObject* object) { forzenObjects.insert(object); }
		static bool isFrozen(SceneObject* object) { return forzenObjects.contains(object); }

        static void handleCollisions(utl::svec<SceneObject>& sceneObjects);
        static void debugDrawCollisions(const utl::svec<SceneObject>& sceneObjects, const Camera& camera);
    };
}
