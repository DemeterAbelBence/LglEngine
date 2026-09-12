#pragma once

import utl;
import glm;

#include <collision  //SphereCollider.hxx>
#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/SphereMesh.hxx>

namespace lgl {
class Sphere : public SceneObject {
 private:
  float m_radius;

 public:
  Sphere(float radius);
};
}  // namespace lgl
