#pragma once

import utl;
import glm;
import std;

#include <collision/Collider.hxx>
#include <collision/SphereCollider.hxx>
#include <collision/TerrainCollider.hxx>

namespace lgl {
/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class CuboidCollider : public Collider {
 public:
  /**
   * @brief lorem ipsum
   *
   * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
   * dolore magna aliqua.
   */
  struct Side {
    std::array<glm::vec3, 4> points;  ///< lorem ipsum
    glm::vec3 normal;                 ///< lorem ipsum
  };

  /**
   * @brief lorem ipsum
   *
   * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
   * dolore magna aliqua.
   */
  struct Edge {
    glm::vec3 point;   ///< lorem ipsum
    glm::vec3 vector;  ///< lorem ipsum
  };

  /**
   * @brief lorem ipsum
   *
   * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
   * dolore magna aliqua.
   */
  struct CuboidData {
    std::array<glm::vec3, 8> points;  ///< lorem ipsum
    std::array<Side, 6> sides;        ///< lorem ipsum
  };

 private:
  CuboidData baseData;   ///< lorem ipsum
  CuboidData transData;  ///< lorem ipsum

 public:
  inline static float s_vertexFaceThreshold = 1.0f;  ///< lorem ipsum
  inline static float s_edgeEdgeThreshold = 1.0f;    ///< lorem ipsum

 private:
  void createCorners();
  void createSides();
  Side createSide(const std::array<std::uint32_t, 4>& indices) const;

 private:
  std::vector<glm::vec3> getEdgesOf(const CuboidCollider& collider) const;
  std::optional<ContactData> calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb,
                                                       float b) const;
  std::vector<ContactData> edgeEdgeCollision(const CuboidCollider& collidee) const;
  bool calculateSideIntersection(glm::vec3 point, const Side& side) const;
  std::vector<ContactData> vertexFaceCollision(const CuboidCollider& collidee) const;

 public:
  CuboidCollider();
  CuboidCollider(const CuboidCollider& c);
  CuboidCollider(CuboidCollider&& c) noexcept = delete;

  CuboidCollider& operator=(const CuboidCollider& c);

  const CuboidData& getBaseData() const { return baseData; }
  const CuboidData& getTransData() const { return transData; }

  std::vector<glm::vec3> getSideDrawData(std::uint32_t sideIndex);
  glm::vec3 calculateSideCenter(const Side& side) const;

  void updateTransformations() override;

  std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

  virtual std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee,
                                                  const ContactData& contact) const override;
  virtual std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee,
                                                  const ContactData& contact) const override;
  virtual std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee,
                                                  const ContactData& contact) const override;
};
}  // namespace lgl
