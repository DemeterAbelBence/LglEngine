#include "CollisionHandlerV1.hxx"

namespace lgl {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
std::string CollisionHandlerV1::contactTypeToString(ContactType type) {
  switch (type) {
    case ContactType::COLLIDING:
      return "COLLIDING";
    case ContactType::SEPARATING:
      return "SEPARATING";
    case ContactType::RESTING:
      return "RESTING";
    default:
      throw std::runtime_error("Invalid ContactType enum value");
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
float CollisionHandlerV1::calculateRelativeVelocityOf(const CONTACT& contact) {
  // Extract contact information from the CONTACT tuple.
  std::uint32_t contactIndex = std::get<0>(contact);
  const auto& contactData = std::get<1>(contact);
  SceneObject* colliderObject = std::get<2>(contact);
  SceneObject* collideeObject = std::get<3>(contact);

  // Get the physics body data for both the collider and collidee objects.
  ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->m_body);
  ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->m_body);

  // Calculate the relative velocity at the contact point and determine the
  // contact type (colliding, separating, or resting).
  glm::vec3 ra = contactData.point - A->X;
  glm::vec3 rb = contactData.point - B->X;
  glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
  glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
  float vreln = glm::dot(contactData.normal, velpa - velpb);

  return vreln;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
CollisionHandlerV1::ContactType CollisionHandlerV1::getContactType(float relativeVelocity) {
  if (relativeVelocity > s_contactBias) {
    return ContactType::SEPARATING;
  }
  if (relativeVelocity > -s_contactBias) {
    return ContactType::RESTING;
  } else {
    return ContactType::COLLIDING;
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
std::vector<CollisionHandlerV1::CONTACT> CollisionHandlerV1::calculateContactsWithBisection(
    std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  // Initial collision resolution
  float deltaTime = Time::s_fixedDeltaTime;
  std::vector<CONTACT> contacts;
  resolveCollisions(contacts, sceneObjects);

  // If there are no contacts, there's no need for bisection, so return an
  // empty vector.
  if (contacts.size() == 0) {
    return {};
  }

  // If the maximum penetration depth is already less than the bisection bias,
  // we can consider the contacts as sufficiently resolved and return them.
  float finalDepth = calculateMaxDepth(contacts);
  if (finalDepth < s_bisectionBias) {
    utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
               "Contact depth already below bias, skipping bisection\n\n", finalDepth,
               s_bisectionBias);
    return contacts;
  }

  // Create a snapshot of the current physics state of all scene objects to
  // allow for rolling back during the bisection process.
  std::map<SceneObject*, ribo::BodyData> snapshotPrevious;
  for (auto& sceneObject : sceneObjects) {
    snapshotPrevious[sceneObject.get()] = sceneObject->getPhysicsSolver()->m_previousStates.back();
  }

  std::map<SceneObject*, ribo::BodyData> snapshotCurrent;
  for (auto& sceneObject : sceneObjects) {
    snapshotCurrent[sceneObject.get()] = sceneObject->getPhysicsSolver()->m_body;
  }

  bool sufficientSeparation = false;
  bool reachedLimit = false;

  float currentTime = 0.0f;
  float direction = 1.0f;

  std::uint32_t maxIterations = 20;
  std::uint32_t iterCount = 0;

  // Bisection loop to find the time of impact with sufficient separation
  // between objects.
  while (!sufficientSeparation) {
    // calculate the midpoint time between the last known collision time and
    // the current time.
    deltaTime *= 0.5f;
    currentTime += direction * deltaTime;

    // roll back all scene objects to the snapshot state and step their
    // physics by the current time.
    for (auto& sceneObject : sceneObjects) {
      sceneObject->getPhysicsSolver()->m_body = snapshotPrevious[sceneObject.get()];
    }
    for (auto& sceneObject : sceneObjects) {
      sceneObject->stepPhysicsBy(currentTime);
      sceneObject->updateTransformations();
    }

    // resolve collisions at the current time step to get the contacts and
    // calculate the maximum penetration depth.
    contacts.clear();
    resolveCollisions(contacts, sceneObjects);
    if (contacts.size() > 0) {
      float maxDepth = calculateMaxDepth(contacts);
      if (maxDepth < s_bisectionBias) {
        sufficientSeparation = true;
        finalDepth = maxDepth;
        utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
                   "Bisection converged in {} iterations\n", iterCount);
      }

      direction = -1.0f;
    } else {
      direction = 1.0f;
    }

    if (iterCount++ > maxIterations) {
      utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
                 "Bisection reached limit of {} iterations\n", maxIterations);
      reachedLimit = true;
      sufficientSeparation = true;
    }
  }

  if (reachedLimit) {
    for (auto& sceneObject : sceneObjects) {
      sceneObject->getPhysicsSolver()->m_body = snapshotCurrent[sceneObject.get()];
    }

    contacts.clear();
    resolveCollisions(contacts, sceneObjects);
    finalDepth = calculateMaxDepth(contacts);
    m_bisectedTime = -1.0f;
  } else {
    utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
               "Bisection final depth: {:.6f}\n\n", finalDepth);
    utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
               "Final time after bisection: {:.6f} seconds\n", currentTime);
    m_bisectedTime = currentTime;
  }

  // Log the final contacts after bisection with their positions and normals
  // for debugging purposes.
  for (const auto& contact : contacts) {
    glm::vec3 pos = std::get<1>(contact).point;
    glm::vec3 nor = std::get<1>(contact).normal;

    utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_INFO,
               "Contact{}: collider={}, collidee={}, point=({:.2f}, {:.2f}, "
               "{:.2f}), normal=({:.2f}, {:.2f}, {:.2f})\n",
               std::get<0>(contact), std::get<2>(contact)->getName(),
               std::get<3>(contact)->getName(), pos.x, pos.y, pos.z, nor.x, nor.y, nor.z);
  }

  utl::logIf(s_enableBisectionLog, utl::BISECTION_LOGS, utl::LGL_EMPTY,
             "\n---------------------------------------------------------------\n\n");

  return contacts;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::applyImpulses() {
  utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
             "Applying impulses to objects based on {} contacts:\n", m_currentContacts.size());

  std::uint32_t collidingContactsCount = 0;
  for (const auto& contact : m_currentContacts) {
    float vreln = calculateRelativeVelocityOf(contact);

    utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
               "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n", std::get<0>(contact),
               std::get<2>(contact)->getName(), std::get<3>(contact)->getName(),
               contactTypeToString(getContactType(vreln)), vreln);

    if (getContactType(vreln) == ContactType::COLLIDING) {
      collidingContactsCount++;
    }
  }

  if (collidingContactsCount == 0) {
    utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
               "No colliding contacts found, skipping impulse resolution\n\n");
    return;
  }

  bool hadCollidingContact = true;
  std::uint32_t iterCount = 0;

  // Loop to apply impulses iteratively until there are no more colliding
  // contacts or a maximum number of iterations is reached.
  while (hadCollidingContact) {
    hadCollidingContact = false;

    // If the number of iterations exceeds 20, log a message and break out of
    // the loop.
    if (iterCount++ > 20) {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
                 "Impulse resolution reached limit of 20 iterations\n\n", iterCount);
      return;
    }

    // Iterate through each contact and apply impulses based on the relative
    // velocity and contact type.
    for (const auto& contact : m_currentContacts) {
      // Extract contact information from the CONTACT tuple.
      std::uint32_t contactIndex = std::get<0>(contact);
      const auto& contactData = std::get<1>(contact);
      SceneObject* colliderObject = std::get<2>(contact);
      SceneObject* collideeObject = std::get<3>(contact);

      // Get the physics body data for both the collider and collidee
      // objects.
      ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->m_body);
      ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->m_body);

      // Calculate the relative velocity at the contact point and determine
      // the contact type (colliding, separating, or resting).
      glm::vec3 ra = contactData.point - A->X;
      glm::vec3 rb = contactData.point - B->X;
      glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
      glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
      float vreln = glm::dot(contactData.normal, velpa - velpb);

      // If the contact is colliding, calculate the impulse to apply based on
      // the relative velocity, elasticity, etc.
      ContactType contactType = getContactType(vreln);
      if (contactType == ContactType::COLLIDING) {
        glm::vec3 term1an = A->Iinv * glm::cross(ra, contactData.normal);
        glm::vec3 term1bn = B->Iinv * glm::cross(rb, contactData.normal);
        float term2an = glm::dot(contactData.normal, glm::cross(term1an, ra));
        float term2bn = glm::dot(contactData.normal, glm::cross(term1bn, rb));
        float numerator = -(1.0f + s_elasticity) * vreln;
        float denominator = A->invMass + B->invMass + term2an + term2bn;
        float j = numerator / denominator;

        glm::vec3 impForce = j * contactData.normal;
        glm::vec3 impTorqueA = glm::cross(ra, impForce);
        glm::vec3 impTorqueB = glm::cross(rb, impForce);

        A->P += impForce;
        A->vel = A->P * A->invMass;
        A->L += impTorqueA;
        A->omega = A->Iinv * A->L;

        B->P -= impForce;
        B->vel = B->P * B->invMass;
        B->L -= impTorqueB;
        B->omega = B->Iinv * B->L;

        // Keep looping to apply impulses until there are no more colliding
        // contacts
        hadCollidingContact = true;
      }
    }
  }

  utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
             "Colliding contact eliminated in {} iterations\n\n", iterCount);
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::reclassifyContacts(
    std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
             "Reclassifying objects based on {} contacts:\n", m_currentContacts.size());

  // Clear the restingContacts vector and reclassify each contact after
  // impulses were applied during colliding contact elimination.
  m_restingContacts.clear();
  for (const auto& contact : m_currentContacts) {
    std::string colliderName = std::get<2>(contact)->getName();
    std::string collideeName = std::get<3>(contact)->getName();
    float vreln = calculateRelativeVelocityOf(contact);
    ContactType contactType = getContactType(vreln);

    if (contactType == ContactType::COLLIDING) {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_WARN,
                 "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
                 std::get<0>(contact), colliderName, collideeName, contactTypeToString(contactType),
                 vreln);
    }

    if (contactType == ContactType::SEPARATING) {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
                 "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
                 std::get<0>(contact), colliderName, collideeName, contactTypeToString(contactType),
                 vreln);
    }

    if (contactType == ContactType::RESTING) {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
                 "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
                 std::get<0>(contact), colliderName, collideeName, contactTypeToString(contactType),
                 vreln);

      m_restingContacts.push_back(contact);
    }
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::computeRestingContactMatrix(eig::matd& matrix) {
  auto computeElementAt = [](const CONTACT& ci, const CONTACT& cj) {
    SceneObject* cia = std::get<2>(ci);
    SceneObject* cib = std::get<3>(ci);
    SceneObject* cja = std::get<2>(cj);
    SceneObject* cjb = std::get<3>(cj);

    if ((cia != cja) && (cib != cjb) && (cia != cjb) && (cib != cja)) {
      return 0.0f;
    }

    Collider::ContactData contactDataI = std::get<1>(ci);
    Collider::ContactData contactDataJ = std::get<1>(cj);

    ribo::BodyData* A = &(cia->getPhysicsSolver()->m_body);
    ribo::BodyData* B = &(cib->getPhysicsSolver()->m_body);

    glm::vec3 ni = contactDataI.normal;
    glm::vec3 nj = contactDataJ.normal;
    glm::vec3 pi = contactDataI.point;
    glm::vec3 pj = contactDataJ.point;
    glm::vec3 ra = pi - A->X;
    glm::vec3 rb = pi - B->X;

    glm::vec3 force_on_a = glm::nullvec;
    if (cja == cia) {
      force_on_a = nj;
    } else if (cjb == cia) {
      force_on_a = -nj;
    }
    glm::vec3 torque_on_a = glm::cross(pj - A->X, force_on_a);

    glm::vec3 force_on_b = glm::nullvec;
    if (cja == cib) {
      force_on_b = nj;
    } else if (cjb == cib) {
      force_on_b = -nj;
    }
    glm::vec3 torque_on_b = glm::cross(pj - B->X, force_on_b);

    glm::vec3 a_linear = force_on_a * A->invMass;
    glm::vec3 a_angular = glm::cross(A->Iinv * torque_on_a, ra);

    glm::vec3 b_linear = force_on_b * B->invMass;
    glm::vec3 b_angular = glm::cross(B->Iinv * torque_on_b, rb);

    return glm::dot(ni, a_linear + a_angular - b_linear - b_angular);
  };

  int n = static_cast<int>(m_restingContacts.size());
  matrix.resize(n, n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      float element = computeElementAt(m_restingContacts[i], m_restingContacts[j]);
      eig::setFromFloatAt(matrix, i, j, element);
    }
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::computeRestingContactVector(eig::vecd& vector) {
  int n = static_cast<int>(m_restingContacts.size());
  vector.resize(n);
  int idx = 0;
  for (const auto& contact : m_restingContacts) {
    int contactIndex = std::get<0>(contact);
    const auto& contactData = std::get<1>(contact);
    SceneObject* colliderObject = std::get<2>(contact);
    SceneObject* collideeObject = std::get<3>(contact);

    ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->m_body);
    ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->m_body);

    glm::vec3 n = contactData.normal;
    glm::vec3 ra = contactData.point - A->X;
    glm::vec3 rb = contactData.point - B->X;

    glm::vec3 f_ext_a = A->force;
    glm::vec3 f_ext_b = B->force;
    glm::vec3 t_ext_a = A->torque;
    glm::vec3 t_ext_b = B->torque;

    glm::vec3 a_ext_part = f_ext_a * A->invMass + glm::cross((A->Iinv * t_ext_a), ra);
    glm::vec3 b_ext_part = f_ext_b * B->invMass + glm::cross((B->Iinv * t_ext_b), rb);

    glm::vec3 a_vel_term1 = glm::cross(A->omega, glm::cross(A->omega, ra));
    glm::vec3 a_vel_term2 = glm::cross(A->Iinv * glm::cross(A->L, A->omega), ra);
    glm::vec3 a_vel_part = a_vel_term1 + a_vel_term2;

    glm::vec3 b_vel_term1 = glm::cross(B->omega, glm::cross(B->omega, rb));
    glm::vec3 b_vel_term2 = glm::cross(B->Iinv * glm::cross(B->L, B->omega), rb);
    glm::vec3 b_vel_part = b_vel_term1 + b_vel_term2;

    glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
    glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);

    glm::vec3 ndot;
    if (contactData.isVertexFace) {
      ndot = glm::cross(B->omega, n);
    } else {
      glm::vec3 ea = contactData.edgeA[1];
      glm::vec3 eb = contactData.edgeB[1];

      glm::vec3 eadot = glm::cross(A->omega, ea);
      glm::vec3 ebdot = glm::cross(B->omega, eb);
      glm::vec3 n1 = glm::cross(ea, eb);
      glm::vec3 z = glm::cross(eadot, eb) + glm::cross(ea, ebdot);
      float l = glm::length(n1);
      n1 = glm::normalize(n1);

      ndot = (z - glm::cross(glm::cross(z, n1), n1)) / l;
    }

    float k1 = glm::dot(n, a_ext_part - b_ext_part + a_vel_part - b_vel_part);
    float k2 = 2.0f * glm::dot(ndot, velpa - velpb);

    eig::setFromFloatAt(vector, idx++, k1 + k2);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::resolveRestingContacts() {
  // If there are resting contacts, we need to solve for the contact forces
  // that will prevent interpenetration and allow the objects to rest on each
  // other without sinking or bouncing.

  if (!m_restingContacts.empty()) {
    // Compute the contact matrix for the resting contacts
    eig::matd A;
    computeRestingContactMatrix(A);
    utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO, "Resting contact matrix A:\n");
    eig::logMatrix(A, s_enableContactLog);

    // Compute the contact vector b for the resting contacts
    eig::vecd b;
    computeRestingContactVector(b);
    utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO, "Resting contact vector b:\n");
    eig::logVector(b, s_enableContactLog);

    // Check if the contact matrix A is positive semi-definite, which is a
    // requirement for solving the quadratic program for contact forces.
    if (eig::isPSD(A, s_PSDTolerance)) {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_OK,
                 "Resting contact matrix is positive semi-definite, can solve "
                 "QP :)\n");

      eig::vecd forces;
      if (qp::solveBaraffContactForces(A, b, forces)) {
        utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
                   "Contact forces solved:\n");
        eig::logVector(forces, s_enableContactLog);

        for (int i = 0; i < m_restingContacts.size(); ++i) {
          // Extract contact information from the CONTACT tuple.
          CONTACT& contact = m_restingContacts[i];
          const auto& contactData = std::get<1>(contact);
          SceneObject* colliderObject = std::get<2>(contact);
          SceneObject* collideeObject = std::get<3>(contact);

          // Track the maximum resting force applied for logging purposes.
          float forceMagnitude = eig::getAsFloatAt(forces, i);
          if (forceMagnitude > s_maxRestingForce) {
            utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_WARN,
                       "Resting force calculation diverged or too large\n");
            continue;
          }

          // Apply the contact force to both the collider and collidee
          // objects in opposite directions along the contact normal.
          glm::vec3 force = forceMagnitude * contactData.normal;
          colliderObject->getPhysicsSolver()->m_body.force += force;
          colliderObject->getPhysicsSolver()->m_body.torque +=
              glm::cross(contactData.point - colliderObject->getPhysicsSolver()->m_body.X, force);
          collideeObject->getPhysicsSolver()->m_body.force -= force;
          collideeObject->getPhysicsSolver()->m_body.torque -=
              glm::cross(contactData.point - collideeObject->getPhysicsSolver()->m_body.X, force);
        }
      }
    } else {
      utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_WARN,
                 "Resting contact matrix is not positive semi-definite, cannot "
                 "solve QP\n");
    }
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
std::vector<CollisionHandlerV1::CONTACT> CollisionHandlerV1::findInterpenetratingContacts(
    const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  std::vector<CONTACT> contacts;
  resolveCollisions(contacts, sceneObjects);
  float maxDepth = calculateMaxDepth(contacts);

  m_deepestInterpenetration = maxDepth;
  utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_INFO,
             "Deepest interpenetration: {:.6f}\n", m_deepestInterpenetration);
  utl::logIf(s_enableContactLog, utl::CONTACT_LOGS, utl::LGL_EMPTY,
             "\n---------------------------------------------------------------\n\n");

  return contacts;
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::resolveInterpenetrations(
    const std::vector<CollisionHandlerV1::CONTACT>& contacts) {
  std::unordered_map<std::uint64_t, std::vector<CONTACT>> contactsPerPair;
  std::unordered_map<std::uint64_t, std::tuple<SceneObject*, SceneObject*>> objectsPerPair;

  for (const auto& contact : contacts) {
    std::uint64_t pairHash = utl::makePointerPairHash(std::get<2>(contact), std::get<3>(contact));
    contactsPerPair[pairHash].push_back(contact);
    objectsPerPair[pairHash] = {std::get<2>(contact), std::get<3>(contact)};
  }

  for (const auto& [pairHash, pairContacts] : contactsPerPair) {
    auto object1 = std::get<0>(objectsPerPair[pairHash]);
    auto object2 = std::get<1>(objectsPerPair[pairHash]);

    Collider::ContactData deepestContactData;
    float maxDist = 0.0f;
    for (const auto& contact : pairContacts) {
      Collider::ContactData contactData = std::get<1>(contact);
      if (contactData.depth.has_value()) {
        float depthLength = glm::length(*contactData.depth);
        if (depthLength > maxDist) {
          maxDist = depthLength;
          deepestContactData = contactData;
        }
      }
    }

    if (maxDist == 0.0f) {
      break;
    }

    glm::vec3 object1Pos = object1->getPhysicsSolver()->m_body.X;
    glm::vec3 object2Pos = object2->getPhysicsSolver()->m_body.X;

    glm::vec3 point = deepestContactData.point;
    glm::vec3 normal = deepestContactData.normal;
    glm::vec3 displacement = *deepestContactData.depth;

    SceneObject* objectToDisplace;
    if (glm::dot(object1Pos - point, normal) > 0.0f) {
      objectToDisplace = object1;
    } else if (glm::dot(object2Pos - point, normal) > 0.0f) {
      objectToDisplace = object2;
    } else {
      continue;
    }

    objectToDisplace->getPhysicsSolver()->m_body.X += displacement;
    objectToDisplace->updateTransformations();
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::readConfig(const utl::LglConf& lglConf) {
  s_enableRestingForces = lglConf.getBooleanConfig("enableRestingForces");
  s_enableImpulses = lglConf.getBooleanConfig("enableImpulses");
  s_enableBisection = lglConf.getBooleanConfig("enableBisection");

  s_enableBisectionLog = lglConf.getBooleanConfig("enableBisectionLog");
  s_enableContactLog = lglConf.getBooleanConfig("enableContactLog");
  s_enablePhysicsLog = lglConf.getBooleanConfig("enablePhysicsLog");

  s_elasticity = lglConf.getFloatConfig("elasticity");
  s_bisectionBias = lglConf.getFloatConfig("bisectionBias");
  s_contactBias = lglConf.getFloatConfig("contactBias");
  s_PSDTolerance = lglConf.getFloatConfig("PSDtolerance");
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void CollisionHandlerV1::handleCollisions(std::vector<std::shared_ptr<SceneObject>>& sceneObjects) {
  // Clear the current contacts and calculate new contacts
  m_currentContacts.clear();
  if (s_enableBisection) {
    m_currentContacts = calculateContactsWithBisection(sceneObjects);
  } else {
    resolveCollisions(m_currentContacts, sceneObjects);
  }

  // Handle the calculated contacts
  if (s_enableImpulses) {
    applyImpulses();
  }
  if (s_enableRestingForces) {
    reclassifyContacts(sceneObjects);
    resolveRestingContacts();
  }

  // Stepping the time left after bisection to reach the end of the fixed time
  // step
  if (m_bisectedTime >= 0.0f) {
    float remainingTime = Time::s_fixedDeltaTime - m_bisectedTime;
    for (auto& sceneObject : sceneObjects) {
      sceneObject->stepPhysicsBy(remainingTime);
    }
    m_bisectedTime = -1.0f;
  }

  // After all physics calculations resolve unrealistic interpenetrations
  const std::vector<CONTACT>& interpenetratingContacts = findInterpenetratingContacts(sceneObjects);
  if (s_enableDepenetration) {
    resolveInterpenetrations(interpenetratingContacts);
  }

  if (s_enablePhysicsLog) {
    utl::log(utl::PHYSICS_LOGS, utl::LGL_INFO, "Post-collision state of all objects:\n\n");
    logPhysicsStateOfObjects(false, sceneObjects);
  }
}
}  // namespace lgl