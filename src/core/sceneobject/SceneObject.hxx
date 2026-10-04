#pragma once

import utl;
import glm;
import std;

#include <collision/Collider.hxx>
#include <collision/NullCollider.hxx>
#include <gl/FrameBuffer.hxx>
#include <math/ribo.hxx>
#include <visual/common/Light.hxx>
#include <visual/mesh/Mesh.hxx>
#include <visual/mesh/ModelMesh.hxx>

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
namespace lgl {
class SceneObject {
 protected:
  std::shared_ptr<Mesh> m_mesh;                          ///< lorem ipsum
  std::shared_ptr<Collider> m_collider;                  ///< lorem ipsum
  std::shared_ptr<ribo::PhysicsSolver> m_physicsSolver;  ///< lorem ipsum
  std::shared_ptr<Transformation> m_transformation;      ///< lorem ipsum

 private:
  std::string m_name;  ///< lorem ipsum

 public:
  SceneObject();
  SceneObject(float size, std::shared_ptr<ModelMesh> mesh);

  const std::shared_ptr<Mesh>& getMesh() const { return m_mesh; }
  const std::shared_ptr<Collider>& getCollider() const { return m_collider; }
  const std::shared_ptr<ribo::PhysicsSolver>& getPhysicsSolver() const { return m_physicsSolver; }
  const std::shared_ptr<Transformation>& getTransformation() const { return m_transformation; }
  const std::string getName() const { return m_name; }

  void setMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
  void setProgram(std::shared_ptr<GpuProgram> program);
  void setMaterial(std::shared_ptr<Material> material);
  void setTextures(const std::vector<std::shared_ptr<Texture>>& textures);
  void setName(const std::string& name) { m_name = name; }

  void translate(glm::vec3 transalte);
  void scale(glm::vec3 scale);
  void setRotation(glm::mat4 R);
  void updateTransformations();

  virtual std::shared_ptr<SceneObject> clone() { return std::shared_ptr<SceneObject>(); }
  virtual ribo::BodyData initializePhysics() { return ribo::BodyData{}; }
  virtual void updateRigidBody() {}
  virtual void stepPhysicsBy(float dt);
  virtual void draw(const Camera& camera) const;
  virtual void handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L);
};
}  // namespace lgl