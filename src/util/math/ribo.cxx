#include "ribo.hxx"

namespace lgl {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 *
 * @param p1 lorem ipsum
 * @param p2 lorem ipsum
 *
 * @return lorem ipsum
 */
glm::mat3 ribo::star(glm::vec3 a) {
  return glm::mat3(glm::vec3(0.0f, -a[2], a[1]), glm::vec3(a[2], 0.0f, -a[0]),
                   glm::vec3(-a[1], a[0], 0.0f));
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 *
 * @param p1 lorem ipsum
 * @param p2 lorem ipsum
 *
 * @return lorem ipsum
 */
glm::mat3 ribo::orthonormalize(const glm::mat3& R) {
  glm::vec3 cx = glm::vec3(R[0][0], R[1][0], R[2][0]);
  glm::vec3 cy = glm::vec3(R[0][1], R[1][1], R[2][1]);
  glm::vec3 cz = glm::vec3(R[0][2], R[1][2], R[2][2]);

  glm::vec3 rx = glm::normalize(cx);
  glm::vec3 ry = glm::normalize(glm::cross(cz, rx));
  glm::vec3 rz = glm::normalize(glm::cross(rx, ry));

  // return glm::mat3(glm::vec3(rx.x, ry.x, rz.x), glm::vec3(rx.y, ry.y, rz.y), glm::vec3(rx.z,
  // ry.z, rz.z));
  return R;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 *
 * @param p1 lorem ipsum
 * @param p2 lorem ipsum
 *
 * @return lorem ipsum
 */
void ribo::serializeBodyDataInto(const std::string& filename, const BodyData& body) {
  try {
    utl::createDirectories(filename);
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    auto writeFloat = [&](float v) {
      file.write(reinterpret_cast<const char*>(&v), sizeof(float));
    };
    auto writeVec3 = [&](const glm::vec3& v) {
      file.write(reinterpret_cast<const char*>(&v[0]), 3 * sizeof(float));
    };
    auto writeMat3 = [&](const glm::mat3& m) {
      file.write(reinterpret_cast<const char*>(&m[0][0]), 9 * sizeof(float));
    };

    writeFloat(body.invMass);
    writeMat3(body.Ibody);
    writeMat3(body.Ibodyinv);
    writeVec3(body.X);
    writeMat3(body.R);
    writeVec3(body.P);
    writeVec3(body.L);
    writeMat3(body.Iinv);
    writeVec3(body.vel);
    writeVec3(body.omega);
    writeVec3(body.force);
    writeVec3(body.torque);

    file.close();
  } catch (const std::exception& error) {
    throw error;
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 *
 * @param p1 lorem ipsum
 *
 * @return lorem ipsum
 */
ribo::BodyData ribo::deserializeBodyDataFrom(const std::string& filename) {
  try {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    BodyData body;

    auto readFloat = [&](float& v) { file.read(reinterpret_cast<char*>(&v), sizeof(float)); };
    auto readVec3 = [&](glm::vec3& v) {
      file.read(reinterpret_cast<char*>(&v[0]), 3 * sizeof(float));
    };
    auto readMat3 = [&](glm::mat3& m) {
      file.read(reinterpret_cast<char*>(&m[0][0]), 9 * sizeof(float));
    };

    readFloat(body.invMass);
    readMat3(body.Ibody);
    readMat3(body.Ibodyinv);
    readVec3(body.X);
    readMat3(body.R);
    readVec3(body.P);
    readVec3(body.L);
    readMat3(body.Iinv);
    readVec3(body.vel);
    readVec3(body.omega);
    readVec3(body.force);
    readVec3(body.torque);

    file.close();
    return body;
  } catch (const std::exception& error) {
    throw error;
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
ribo::PhysicsSolver::PhysicsSolver(const PhysicsSolver& other)
    : m_body(other.m_body), m_initial(other.m_initial), m_previousStates(other.m_previousStates) {}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
ribo::PhysicsSolver& ribo::PhysicsSolver::operator=(const PhysicsSolver& other) {
  if (this != &other) {
    m_body = other.m_body;
    m_initial = other.m_initial;
    m_previousStates = other.m_previousStates;
  }
  return *this;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void ribo::PhysicsSolver::rollbackToPrevious() {
  if (!m_previousStates.empty()) {
    m_body = m_previousStates.back();
    m_previousStates.pop();
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void ribo::PhysicsSolver::initForces() {
  if (m_body.invMass != 0.0f) {
    m_body.force = glm::vec3(0.0f, -9.81f, 0.0f) / m_body.invMass;
  } else {
    m_body.force = glm::vec3(0.0f);
  }
  m_body.torque = glm::vec3(0.0f);
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void ribo::PhysicsSolver::updateVelocities() {
  m_body.Iinv = m_body.R * m_body.Ibodyinv * glm::transpose(m_body.R);
  m_body.omega = m_body.Iinv * m_body.L;

  m_body.vel = m_body.invMass * m_body.P;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 *
 * @param p1 lorem ipsum
 */
void ribo::PhysicsSolver::updateState(float dt) {
  m_previousStates.push(m_body);
  if (m_previousStates.size() > 10) {
    m_previousStates.pop();
  }

  m_body.P += m_body.force * dt;
  m_body.L += m_body.torque * dt;

  updateVelocities();

  m_body.X += m_body.vel * dt;
  m_body.R += star(m_body.omega) * m_body.R * dt;

  if (s_enableOrthonormalization) {
    m_body.R = orthonormalize(m_body.R);
  }
}
}  // namespace lgl