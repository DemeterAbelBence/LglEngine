#pragma once

import utl;
import glm;
import qp;
import std;

#include <collision/CuboidCollider.hxx>
#include <collision/NullCollider.hxx>
#include <collision/SphereCollider.hxx>
#include <collision/TerrainCollider.hxx>
#include <gl/DebugDrawer.hxx>
#include <sceneobject/SceneObject.hxx>

namespace lgl {
class CollisionHandler {
 public:
  inline static bool enableDebug = true;
  inline static bool enableBisection = true;
  inline static bool enableImpulses = true;
  inline static bool enableRestingForces = true;
  inline static bool enableDepenetration = false;
  inline static bool pushApartOnce = false;
  inline static bool enableContactLog = false;
  inline static bool enableBisectionLog = false;
  inline static bool enablePhysicsLog = false;
  inline static bool drawVectors = false;
  inline static bool logStatesOnce = false;

  inline static float elasticity = 0.3f;
  inline static float bisectionBias = 0.0001f;
  inline static float contactBias = 0.005f;
  inline static float PSDTolerance = 1e-6f;
  inline static float depthBias = 0.000001f;
  inline static float maxRestingForce = 2143289344.0f / 2.0f;

  enum ContactType { COLLIDING, SEPARATING, RESTING };

  using CONTACT = std::tuple<std::uint32_t, Collider::ContactData, SceneObject*,
                             SceneObject*>;
  inline static std::vector<CONTACT> currentContacts = std::vector<CONTACT>();
  inline static std::vector<CONTACT> restingContacts = std::vector<CONTACT>();

  inline static bool logContacts = false;
  inline static float bisectedTime = -1.0f;

 private:
  // Calculate contacts
  static void resolveCollisions(
      std::vector<CONTACT>& contacts,
      const std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  static std::vector<CONTACT> calculateContactsWithBisection(
      std::vector<std::shared_ptr<SceneObject>>& sceneObjects);

  // Impulse resolution
  static ContactType getContactType(float relativeVelocity);
  static void applyImpulses();
  static void reclassifyContacts(
      std::vector<std::shared_ptr<SceneObject>>& sceneObjects);

  // Resting contact resolution
  static void computeRestingContactMatrix(eig::matd& matrix);
  static void computeRestingContactVector(eig::vecd& vector);
  static void resolveRestingContacts();

  // Interpenetration resolution
  static void resolveInterpenetrations(
      std::vector<std::shared_ptr<SceneObject>>& sceneObjects);

  // Debug and draw
  static void debugContacts(const Camera& camera);
  static void drawCollidersOf(
      const std::vector<std::shared_ptr<SceneObject>>& sceneObjects,
      const Camera& camera);

 public:
  // Main entry point
  static void handleCollisions(
      std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  static void debugDrawCollisions(
      const std::vector<std::shared_ptr<SceneObject>>& sceneObjects,
      const Camera& camera);
};
}  // namespace lgl
