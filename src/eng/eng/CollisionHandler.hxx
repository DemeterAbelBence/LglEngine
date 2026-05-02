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
        inline static bool enableImpulses = true;
        inline static bool enableBisection = true;

        inline static bool enableContactLog = true;
        inline static bool enableBisectionLog = true;

        inline static float elasticity = 0.5f;

        inline static float bisectionBias = 0.00001f;
		inline static float contactBias = 0.005f;
		inline static float PSDTolerance = 1e-6f;

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

    private:
        static utl::vec<CONTACT> calculateContacts(utl::svec<SceneObject>& sceneObjects);
        static ContactType getContactType(float relativeVelocity);
        static void applyImpulses();
        static void reclassifyContacts(utl::svec<SceneObject>& sceneObjects);
		static void resolveRestingContacts();

        static void computeRestingContactMatrix(eig::matd& matrix);
        static void computeRestingContactVector(eig::vecd& vector);

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
