#pragma once

import utl;
import glm;
import qp;
import std;

#include <eng/CollisionHandler.hxx>

namespace lgl {
class CollisionHandlerV1 final : public CollisionHandler {
 public:
  inline static bool s_enableDebug = true;
  inline static bool s_enableBisection = true;
  inline static bool s_enableImpulses = true;
  inline static bool s_enableRestingForces = true;
  inline static bool s_enableDepenetration = false;
  inline static bool s_pushApartOnce = false;
  inline static bool s_enableContactLog = true;
  inline static bool s_enableBisectionLog = false;
  inline static bool s_enablePhysicsLog = false;
  inline static bool s_drawVectors = false;

  inline static float s_elasticity = 0.3f;
  inline static float s_bisectionBias = 0.0001f;
  inline static float s_contactBias = 0.005f;
  inline static float s_PSDTolerance = 1e-6f;
  inline static float s_depthBias = 0.000001f;
  inline static float s_maxRestingForce = 2143289344.0f / 2.0f;

 private:
  float m_bisectedTime = -1.0f;
  float m_deepestInterpenetration = 0.0f;
  std::vector<CONTACT> m_restingContacts;

 private:
  enum ContactType { COLLIDING, SEPARATING, RESTING };

  std::string contactTypeToString(ContactType type);
  float calculateRelativeVelocityOf(const CONTACT& contact);
  ContactType getContactType(float relativeVelocity);
  std::vector<CONTACT> calculateContactsWithBisection(
      std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  void applyImpulses();
  void reclassifyContacts(std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  void computeRestingContactMatrix(eig::matd& matrix);
  void computeRestingContactVector(eig::vecd& vector);
  void resolveRestingContacts();
  std::vector<CONTACT> findInterpenetratingContacts(
      const std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  void resolveInterpenetrations(const std::vector<CONTACT>& contacts);

 public:
  void readConfig(const utl::LglConf& lglConf) override;
  void handleCollisions(std::vector<std::shared_ptr<SceneObject>>& sceneObjects) override;
};
}  // namespace lgl
