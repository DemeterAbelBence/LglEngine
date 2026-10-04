#pragma once

import utl;
import glm;
import std;

#include <collision/CuboidCollider.hxx>
#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/CuboidMesh.hxx>
#include <visual/mesh/ModelMesh.hxx>

namespace lgl {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class Box final : public SceneObject {
 private:
  bool m_isStationary;			   ///< lorem ipsum
  const float m_massFactor = 1.0f; ///< lorem ipsum

 public:
  Box() : SceneObject() {}
  Box(bool isStationary, std::shared_ptr<ModelMesh> modelMesh);
  Box(bool isStationary);

  std::shared_ptr<SceneObject> clone() override;
  ribo::BodyData initializePhysics() override;
  void updateRigidBody() override;
  void draw(const Camera& camera) const override;
};
}  // namespace lgl