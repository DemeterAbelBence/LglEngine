#include "SceneObject.hxx"

namespace lgl {
/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
SceneObject::SceneObject() {}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
SceneObject::SceneObject(float size, std::shared_ptr<ModelMesh> mesh) {
  m_mesh = mesh;
  m_collider = std::make_shared<NullCollider>();

  auto body = ribo::BodyData{
      0.0f,             // invMass
      glm::mat3(1.0f),  // Ibody
      glm::mat3(1.0f),  // Ibodyinv

      glm::vec3(0.0f),  // X
      glm::mat3(1.0f),  // R
      glm::vec3(0.0f),  // P
      glm::vec3(0.0f),  // L

      glm::mat3(1.0f),  // Iinv
      glm::vec3(0.0f),  // vel
      glm::vec3(0.0f),  // omega

      glm::vec3(0.0f),  // force
      glm::vec3(0.0f)   // torque
  };
  m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(body);

  m_transformation = std::make_shared<Transformation>();
  glm::vec3 scaling = glm::vec3(size, size, size);
  m_transformation->setScaleMatrix(scaling);
  m_collider->setTransformation(m_transformation);

  m_mesh->setTransformation(m_transformation);
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::setProgram(std::shared_ptr<GpuProgram> program) {
  if (m_mesh) {
    m_mesh->setProgram(program);
  } else {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_WARN, "No mesh to set program for SceneObject {}\n", m_name);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::setMaterial(std::shared_ptr<Material> material) {
  if (m_mesh) {
    m_mesh->setMaterial(material);
  } else {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_WARN, "No mesh to set material for SceneObject {}\n", m_name);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::setTextures(const std::vector<std::shared_ptr<Texture>>& textures) {
  if (m_mesh) {
    for (const auto& texture : textures) {
      m_mesh->addTexture(texture);
    }
  } else {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_WARN, "No mesh to set textures for SceneObject {}\n", m_name);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::translate(glm::vec3 translation) {
  m_physicsSolver->m_body.X += translation;
  updateTransformations();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::scale(glm::vec3 scale) {
  m_transformation->setScaleMatrix(scale);
  updateTransformations();
  updateRigidBody();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::setRotation(glm::mat4 R) {
  m_physicsSolver->m_body.R = glm::mat3(R);
  updateTransformations();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::updateTransformations() {
  m_transformation->setTranslationMarix(m_physicsSolver->m_body.X);

  m_transformation->setR(glm::mat4(glm::vec4(m_physicsSolver->m_body.R[0], 0.0f),
                                   glm::vec4(m_physicsSolver->m_body.R[1], 0.0f),
                                   glm::vec4(m_physicsSolver->m_body.R[2], 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

  m_collider->updateTransformations();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::stepPhysicsBy(float dt) {
  m_physicsSolver->updateState(dt);
  updateTransformations();
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::draw(const Camera& camera) const {
  if (m_mesh) {
    m_mesh->draw(camera);
  } else {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_WARN, "No mesh to draw for SceneObject {}\n", m_name);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
void SceneObject::handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L) {
  auto program = m_mesh->getProgram();
  if (program) {
    program->bind();
    program->setVec3("light.position", light.position);
    program->setVec3("light.powerDensity", light.powerDensity);
    program->setMat4("L", L);
    depthMap.bindTexture(*program);
  } else {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_WARN, "No program to handle lighting for SceneObject {}\n", m_name);
  }
}
}  // namespace lgl