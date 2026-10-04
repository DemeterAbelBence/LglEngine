#include "CollisionHandler.hxx"

namespace lgl {
/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandler::logPhysicsStateOfObjects(
    bool toConsole, const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  utl::LogMode logMode = toConsole ? utl::CONSOLE_LOGS : utl::PHYSICS_LOGS;

  utl::log(logMode, utl::LGL_INFO, "Logging physics state of objects\n");
  for (const auto& sceneObject : sceneObjects) {
    if (sceneObject->getName() == "sun") {
      continue;
    }

    const float M = sceneObject->getPhysicsSolver()->m_body.invMass;
    const glm::vec3& F = sceneObject->getPhysicsSolver()->m_body.force;
    const glm::vec3& T = sceneObject->getPhysicsSolver()->m_body.torque;
    const glm::vec3& X = sceneObject->getPhysicsSolver()->m_body.X;
    const glm::vec3& V = sceneObject->getPhysicsSolver()->m_body.vel;
    const glm::vec3& O = sceneObject->getPhysicsSolver()->m_body.omega;
    const glm::vec3& P = sceneObject->getPhysicsSolver()->m_body.P;
    const glm::vec3& L = sceneObject->getPhysicsSolver()->m_body.L;

    utl::log(logMode, utl::LGL_INFO, "\tState of {}:\n", sceneObject->getName());
    utl::log(logMode, utl::LGL_INFO, "\t\tM = {:.6f}\n", M);
    utl::log(logMode, utl::LGL_INFO, "\t\tF = ({:.2f}, {:.2f}, {:.2f})\n", F.x, F.y, F.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tT = ({:.2f}, {:.2f}, {:.2f})\n", T.x, T.y, T.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tX = ({:.2f}, {:.2f}, {:.2f})\n", X.x, X.y, X.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tV = ({:.2f}, {:.2f}, {:.2f})\n", V.x, V.y, V.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tO = ({:.2f}, {:.2f}, {:.2f})\n", O.x, O.y, O.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tP = ({:.2f}, {:.2f}, {:.2f})\n", P.x, P.y, P.z);
    utl::log(logMode, utl::LGL_INFO, "\t\tL = ({:.2f}, {:.2f}, {:.2f})\n", L.x, L.y, L.z);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
float CollisionHandler::calculateMaxDepth(std::vector<CONTACT>& contacts) {
  float maxDepth = 0.0f;
  for (const auto& contact : contacts) {
    auto contactData = std::get<1>(contact);
    if (contactData.depth.has_value()) {
      float depth = glm::length(*contactData.depth);
      maxDepth = glm::max(maxDepth, depth);
    }
  }
  return maxDepth;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandler::resolveCollisions(
    std::vector<CONTACT>& contacts, const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  bool processedAll = false;
  std::vector<std::shared_ptr<SceneObject>> underProcess = sceneObjects;
  std::uint32_t contactIndex = 0;

  while (!processedAll) {
    if (!underProcess.empty()) {
      SceneObject* s1 = underProcess.back().get();
      underProcess.pop_back();

      for (const auto& s2Ptr : underProcess) {
        SceneObject* s2 = s2Ptr.get();

        auto s1_collider = s1->getCollider();
        auto s2_collider = s2->getCollider();

        auto collision1 = s1_collider->collidesWith(*s2_collider);
        for (const auto& contactData : collision1) {
          contacts.push_back({++contactIndex, contactData, s1, s2});
        }

        auto collision2 = s2_collider->collidesWith(*s1_collider);
        for (const auto& contactData : collision2) {
          if (contactData.isVertexFace) {
            contacts.push_back({++contactIndex, contactData, s2, s1});
          }
        }
      }
    } else {
      processedAll = true;
    }
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandler::debugContacts(const Camera& camera, bool drawVectors) {
  std::vector<glm::vec3> contactPoints;
  for (const auto& contact : m_currentContacts) {
    std::uint32_t contactIndex = std::get<0>(contact);
    Collider::ContactData contactData = std::get<1>(contact);
    SceneObject* colliderObject = std::get<2>(contact);
    SceneObject* collideeObject = std::get<3>(contact);

    ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->m_body);
    ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->m_body);

    contactPoints.push_back(contactData.point);
    glm::vec3 ra = contactData.point - A->X;
    glm::vec3 rb = contactData.point - B->X;
    glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
    glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
    glm::vec3 vrel = velpa - velpb;

    if (drawVectors) {
      DebugDrawer::setOverrideZ(1);
      DebugDrawer::setMode(GL_LINES);

      float debugveclen = 1.0f;

      DebugDrawer::setVertexData({contactData.point, contactData.point + debugveclen * velpa});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 0.0f));

      DebugDrawer::setVertexData({contactData.point, contactData.point + debugveclen * velpb});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 1.0f));

      DebugDrawer::setVertexData({contactData.point, contactData.point + debugveclen * vrel});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 1.0f));

      DebugDrawer::setVertexData(
          {contactData.point, contactData.point + debugveclen * contactData.normal});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.3f, 0.0f));
    }

    if (contactData.isVertexFace) {
      glm::vec3 contactPoint1 = contactData.point;
      glm::vec3 contactPoint2 = contactPoint1 + *contactData.depth;

      DebugDrawer::setOverrideZ(1);
      DebugDrawer::setMode(GL_LINES);
      DebugDrawer::setVertexData({contactPoint1, contactPoint2});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 0.0f));
    } else {
      glm::vec3 edgePoint1 = contactData.edgeA[0];
      glm::vec3 edgePoint2 = contactData.edgeB[0];

      DebugDrawer::setOverrideZ(1);
      DebugDrawer::setMode(GL_LINES);
      DebugDrawer::setVertexData({edgePoint1, edgePoint2});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 0.0f));

      DebugDrawer::setMode(GL_POINTS);
      DebugDrawer::setVertexData({edgePoint1});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 1.0f));
      DebugDrawer::setVertexData({edgePoint2});
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 1.0f));
    }
  }

  DebugDrawer::setMode(GL_POINTS);
  DebugDrawer::setOverrideZ(1);
  DebugDrawer::setVertexData(contactPoints);
  DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandler::drawCollidersOf(
    const std::vector<std::shared_ptr<SceneObject>>& sceneObjects, const Camera& camera) {
  for (auto& sceneObject : sceneObjects) {
    auto cuboidCollider = dynamic_cast<CuboidCollider*>(sceneObject->getCollider().get());
    if (cuboidCollider != nullptr) {
      auto p = cuboidCollider->getTransData().points;
      auto points = std::vector<glm::vec3>(p.begin(), p.end());
      DebugDrawer::setMode(GL_POINTS);
      DebugDrawer::setVertexData(points);
      DebugDrawer::setOverrideZ(0);
      DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 0.0f));

      std::vector<std::vector<glm::vec3>> sideData;
      sideData.push_back(cuboidCollider->getSideDrawData(0));
      sideData.push_back(cuboidCollider->getSideDrawData(1));
      sideData.push_back(cuboidCollider->getSideDrawData(2));
      sideData.push_back(cuboidCollider->getSideDrawData(3));
      sideData.push_back(cuboidCollider->getSideDrawData(4));
      sideData.push_back(cuboidCollider->getSideDrawData(5));

      for (const auto& data : sideData) {
        DebugDrawer::setMode(GL_LINES);
        DebugDrawer::setOverrideZ(0);
        DebugDrawer::setVertexData(data);
        DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.4f, 0.4f));
      }
    }
  }
}
}  // namespace lgl