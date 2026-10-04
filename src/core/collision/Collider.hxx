#pragma once

import glm;

#include <math/Transformation.hxx>

namespace lgl {
class CuboidCollider;
class TerrainCollider;
class SphereCollider;

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class Collider {
 public:
  enum GeomType { C_CUBOID, C_SPHERE, C_TERRAIN, C_NULL };
  GeomType geomType = C_NULL;  ///< lorem ipsum

 public:
  /**
   * @brief lorem ipsum
   *
   * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
   * dolore magna aliqua.
   */
  struct ContactData {
    glm::vec3 point;                 ///< lorem ipsum
    glm::vec3 normal;                ///< lorem ipsum
    std::optional<glm::vec3> depth;  ///< lorem ipsum
    bool isVertexFace;               ///< lorem ipsum

    std::array<glm::vec3, 2> edgeA;  ///< lorem ipsum
    std::array<glm::vec3, 2> edgeB;  ///< lorem ipsum

    glm::vec3 facePoint;  ///< lorem ipsum
  };

 protected:
  std::shared_ptr<Transformation> transformation;  ///< lorem ipsum

 protected:
  virtual std::vector<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
  virtual std::vector<ContactData> collidesWith(const TerrainCollider& collider) const = 0;
  virtual std::vector<ContactData> collidesWith(const SphereCollider& collider) const = 0;

  virtual std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const;
  virtual std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const;
  virtual std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const;

 public:
  void setTransformation(std::shared_ptr<Transformation> value) { transformation = value; }
  const std::shared_ptr<Transformation> getTransformation() const { return transformation; }
  Collider::GeomType getColliderType() const { return geomType; }

  std::vector<ContactData> collidesWith(const Collider& collidee) const;
  std::optional<glm::vec3> calculateContactDepthWith(const Collider& collidee, const ContactData& contact);

  virtual void updateTransformations() {};
};
}  // namespace lgl