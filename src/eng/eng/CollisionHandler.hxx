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
  using CONTACT = std::tuple<std::uint32_t, Collider::ContactData, SceneObject*, SceneObject*>;

 protected:
  std::vector<CONTACT> m_currentContacts;

 public:
  void logPhysicsStateOfObjects(bool isLoggingEnabled,
                                const std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  float calculateMaxDepth(std::vector<CONTACT>& contacts);
  void resolveCollisions(std::vector<CONTACT>& contacts,
                         const std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
  void debugContacts(const Camera& camera, bool drawVectors);
  void drawCollidersOf(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects,
                       const Camera& camera);

 public:
  virtual void readConfig(const utl::LglConf& lglConf) = 0;
  virtual void handleCollisions(std::vector<std::shared_ptr<SceneObject>>& sceneObjects) = 0;
};
}  // namespace lgl
