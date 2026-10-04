#pragma once

import utl;
import glm;
import std;

namespace lgl {
namespace ribo {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
struct BodyData {
  float invMass = 0.0f;                  ///< lorem ipsum
  glm::mat3 Ibody = glm::mat3(1.0f);     ///< lorem ipsum
  glm::mat3 Ibodyinv = glm::mat3(1.0f);  ///< lorem ipsum
  glm::vec3 X = glm::vec3(0.0f);         ///< lorem ipsum
  glm::mat3 R = glm::mat3(1.0f);         ///< lorem ipsum
  glm::vec3 P = glm::vec3(0.0f);         ///< lorem ipsum
  glm::vec3 L = glm::vec3(0.0f);         ///< lorem ipsum
  glm::mat3 Iinv = glm::mat3(1.0f);      ///< lorem ipsum
  glm::vec3 vel = glm::vec3(0.0f);       ///< lorem ipsum
  glm::vec3 omega = glm::vec3(0.0f);     ///< lorem ipsum
  glm::vec3 force = glm::vec3(0.0f);     ///< lorem ipsum
  glm::vec3 torque = glm::vec3(0.0f);    ///< lorem ipsum
};

glm::mat3 star(glm::vec3 a);
glm::mat3 orthonormalize(const glm::mat3& R);
void serializeBodyDataInto(const std::string& filename, const BodyData& body);
BodyData deserializeBodyDataFrom(const std::string& filename);

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
class PhysicsSolver {
 public:
  BodyData m_body;                        ///< lorem ipsum
  BodyData m_initial;                     ///< lorem ipsum
  std::queue<BodyData> m_previousStates;  ///< lorem ipsum

 public:
  inline static bool s_enableOrthonormalization = false;  ///< lorem ipsum

 public:
  PhysicsSolver(BodyData body) : m_body(body) {}
  PhysicsSolver(const PhysicsSolver& other);
  PhysicsSolver& operator=(const PhysicsSolver& other);

  void rollbackToPrevious();
  void rollbackToInitial() { m_body = m_initial; }
  void makeStateInitial() { m_initial = m_body; }
  void initForces();
  void updateVelocities();
  void updateState(float dt);
};
}  // namespace ribo
}  // namespace lgl
