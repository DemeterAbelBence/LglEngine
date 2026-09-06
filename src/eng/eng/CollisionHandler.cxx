#include "CollisionHandler.hxx"

namespace lgl {
	// Helper functions
	void logPhysicsStateOfObjects(bool isLoggingEnabled, bool drawSeparator, const utl::svec<SceneObject>& sceneObjects) {
		for (const auto& sceneObject : sceneObjects) {
			if (utl::strEquals(sceneObject->getName(), "sun")) {
				continue;
			}

			glm::vec3 X = sceneObject->getPhysicsSolver()->Body.X;
			glm::vec3 V = sceneObject->getPhysicsSolver()->Body.vel;
			glm::vec3 O = sceneObject->getPhysicsSolver()->Body.omega;
			glm::vec3 P = sceneObject->getPhysicsSolver()->Body.P;
			glm::vec3 L = sceneObject->getPhysicsSolver()->Body.L;

			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "State of {}:\n", sceneObject->getName());
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tM = {:.6f}\n", sceneObject->getPhysicsSolver()->Body.invMass);
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tX = ({:.2f}, {:.2f}, {:.2f})\n", X.x, X.y, X.z);
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tV = ({:.2f}, {:.2f}, {:.2f})\n", V.x, V.y, V.z);
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tO = ({:.2f}, {:.2f}, {:.2f})\n", O.x, O.y, O.z);
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tP = ({:.2f}, {:.2f}, {:.2f})\n", P.x, P.y, P.z);
			Logger::logIf(isLoggingEnabled, Logger::LGL_INFO, "\tL = ({:.2f}, {:.2f}, {:.2f})\n", L.x, L.y, L.z);
		}

		if (drawSeparator) {
			Logger::logIf(isLoggingEnabled, Logger::LGL_EMPTY, "\n-------------------------------------------------------------------\n\n");
		}
	}
	float calculateRelativeVelocityOf(const CollisionHandler::CONTACT& contact) {
		// Extract contact information from the CONTACT tuple.
		utl::uint contactIndex = contact.get<0>();
		const auto& contactData = contact.get<1>();
		SceneObject* colliderObject = contact.get<2>();
		SceneObject* collideeObject = contact.get<3>();

		// Get the physics body data for both the collider and collidee objects.
		ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->Body);
		ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->Body);

		// Calculate the relative velocity at the contact point and determine the contact type (colliding, separating, or resting).
		glm::vec3 ra = contactData.point - A->X;
		glm::vec3 rb = contactData.point - B->X;
		glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
		glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
		float vreln = glm::dot(contactData.normal, velpa - velpb);

		return vreln;
	}
	utl::str contactTypeToString(CollisionHandler::ContactType type) {
		switch (type) {
		case CollisionHandler::ContactType::COLLIDING:
			return "COLLIDING";
		case CollisionHandler::ContactType::SEPARATING:
			return "SEPARATING";
		case CollisionHandler::ContactType::RESTING:
			return "RESTING";
		default:
			throw utl::runtime("Invalid ContactType enum value");
		}
	}
	float calculateMaxDepth(utl::vec<CollisionHandler::CONTACT>& contacts) {
		float maxDepth = 0.0f;
		for (const auto& contact : contacts) {
			auto contactData = contact.get<1>();
			if (contactData.depth.has_value()) {
				float depth = glm::length(*contactData.depth);
				maxDepth = glm::max(maxDepth, depth);
			}
		}
		return maxDepth;
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


	// calculate contacts
	void CollisionHandler::resolveCollisions(utl::vec<CONTACT>& contacts, const utl::svec<SceneObject>& sceneObjects) {
		bool processedAll = false;
		utl::svec<SceneObject> underProcess = sceneObjects;
		utl::uint contactIndex = 0;

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
						contacts.push_back({ ++contactIndex, contactData, s1, s2 });
					}

					auto collision2 = s2_collider->collidesWith(*s1_collider);
					for (const auto& contactData : collision2) {
						if (contactData.isVertexFace) {
							contacts.push_back({ ++contactIndex, contactData, s2, s1 });
						}
					}
				}
			}
			else {
				processedAll = true;
			}
		}
	}
	utl::vec<CollisionHandler::CONTACT> CollisionHandler::calculateContactsWithBisection(utl::svec<SceneObject>& sceneObjects) {
		Logger::setLogMode(Logger::BISECTION_LOGS);

		// Initial collision resolution
		float deltaTime = Time::s_fixedDeltaTime;
		utl::vec<CONTACT> contacts;
		resolveCollisions(contacts, sceneObjects);

		// If there are no contacts, there's no need for bisection, so return an empty vector.
		if (contacts.size() == 0) {
			return {};
		}

		// If the maximum penetration depth is already less than the bisection bias, we can consider the contacts as sufficiently resolved and return them.
		float finalDepth = calculateMaxDepth(contacts);
		if (finalDepth < bisectionBias) {
			Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Contact depth already below bias, skipping bisection\n\n", finalDepth, bisectionBias);
			return contacts;
		}

		// Create a snapshot of the current physics state of all scene objects to allow for rolling back during the bisection process.
		utl::uvmap<SceneObject*, ribo::BodyData> snapshotPrevious;
		for (auto& sceneObject : sceneObjects) {
			snapshotPrevious[sceneObject.get()] = sceneObject->getPhysicsSolver()->PreviousStates.back();
		}

		utl::uvmap<SceneObject*, ribo::BodyData> snapshotCurrent;
		for (auto& sceneObject : sceneObjects) {
			snapshotCurrent[sceneObject.get()] = sceneObject->getPhysicsSolver()->Body;
		}

		bool sufficientSeparation = false;
		bool reachedLimit = false;

		float currentTime = 0.0f;
		float direction = 1.0f;

		utl::uint maxIterations = 20;
		utl::uint iterCount = 0;

		// Bisection loop to find the time of impact with sufficient separation between objects.
		while (!sufficientSeparation) {
			// calculate the midpoint time between the last known collision time and the current time.
			deltaTime *= 0.5f;
			currentTime += direction * deltaTime;

			// roll back all scene objects to the snapshot state and step their physics by the current time.
			for (auto& sceneObject : sceneObjects) {
				sceneObject->getPhysicsSolver()->Body = snapshotPrevious[sceneObject.get()];
			}
			for (auto& sceneObject : sceneObjects) {
				sceneObject->stepPhysicsBy(currentTime);
				sceneObject->updateTransformations();
			}

			// resolve collisions at the current time step to get the contacts and calculate the maximum penetration depth.
			contacts.clear();
			resolveCollisions(contacts, sceneObjects);
			if (contacts.size() > 0) {
				float maxDepth = calculateMaxDepth(contacts);
				if (maxDepth < bisectionBias) {
					sufficientSeparation = true;
					finalDepth = maxDepth;
					Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Bisection converged in {} iterations\n", iterCount);
				}

				direction = -1.0f;
			}
			else {
				direction = 1.0f;
			}

			if (iterCount++ > maxIterations) {
				Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Bisection reached limit of {} iterations\n", maxIterations);
				reachedLimit = true;
				sufficientSeparation = true;
			}
		}

		if (reachedLimit) {
			for (auto& sceneObject : sceneObjects) {
				sceneObject->getPhysicsSolver()->Body = snapshotCurrent[sceneObject.get()];
			}

			contacts.clear();
			resolveCollisions(contacts, sceneObjects);
			finalDepth = calculateMaxDepth(contacts);
			bisectedTime = -1.0f;
		}
		else {
			Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Bisection final depth: {:.6f}\n\n", finalDepth);
			Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Final time after bisection: {:.6f} seconds\n", currentTime);
			bisectedTime = currentTime;
		}

		// Log the final contacts after bisection with their positions and normals for debugging purposes.
		for (const auto& contact : contacts) {

			glm::vec3 pos = contact.get<1>().point;
			glm::vec3 nor = contact.get<1>().normal;

			Logger::logIf(enableBisectionLog, Logger::LGL_INFO, "Contact{}: collider={}, collidee={}, point=({:.2f}, {:.2f}, {:.2f}), normal=({:.2f}, {:.2f}, {:.2f})\n",
				contact.get<0>(), contact.get<2>()->getName(), contact.get<3>()->getName(),
				pos.x, pos.y, pos.z, nor.x, nor.y, nor.z);
		}

		Logger::logIf(enableBisectionLog, Logger::LGL_EMPTY, "\n-------------------------------------------------------------------\n\n");

		return contacts;
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	// Impulse resolution
	CollisionHandler::ContactType CollisionHandler::getContactType(float relativeVelocity) {
		if (relativeVelocity > contactBias) {
			return ContactType::SEPARATING;
		}
		if (relativeVelocity > -contactBias) {
			return ContactType::RESTING;
		}
		else {
			return ContactType::COLLIDING;
		}
	}
	void CollisionHandler::applyImpulses() {
		Logger::setLogMode(Logger::CONTACT_LOGS);
		Logger::logIf(logContacts, Logger::LGL_INFO, "Applying impulses to objects based on {} contacts:\n", currentContacts.size());

		utl::uint collidingContactsCount = 0;
		for(const auto& contact : currentContacts) {
			float vreln = calculateRelativeVelocityOf(contact);

			Logger::logIf(logContacts, Logger::LGL_INFO, "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
				contact.get<0>(), contact.get<2>()->getName(), contact.get<3>()->getName(), contactTypeToString(getContactType(vreln)), vreln);
		
			if (getContactType(vreln) == ContactType::COLLIDING) {
				collidingContactsCount++;
			}
		}

		if (collidingContactsCount == 0) {
			Logger::logIf(logContacts, Logger::LGL_INFO, "No colliding contacts found, skipping impulse resolution\n\n");
			return;
		}

		bool hadCollidingContact = true;
		utl::uint iterCount = 0;

		// Loop to apply impulses iteratively until there are no more colliding contacts or a maximum number of iterations is reached.
		while (hadCollidingContact) {
			hadCollidingContact = false;

			// If the number of iterations exceeds 20, log a message and break out of the loop.
			if (iterCount++ > 20) {
				Logger::logIf(enableContactLog, Logger::LGL_INFO, "Impulse resolution reached limit of 20 iterations\n\n", iterCount);
				return;
			}

			// Iterate through each contact and apply impulses based on the relative velocity and contact type.
			for (const auto& contact : currentContacts) {
				// Extract contact information from the CONTACT tuple.
				utl::uint contactIndex = contact.get<0>();
				const auto& contactData = contact.get<1>();
				SceneObject* colliderObject = contact.get<2>();
				SceneObject* collideeObject = contact.get<3>();

				// Get the physics body data for both the collider and collidee objects.
				ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->Body);
				ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->Body);

				// Calculate the relative velocity at the contact point and determine the contact type (colliding, separating, or resting).
				glm::vec3 ra = contactData.point - A->X;
				glm::vec3 rb = contactData.point - B->X;
				glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
				glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
				float vreln = glm::dot(contactData.normal, velpa - velpb);

				// If the contact is colliding, calculate the impulse to apply based on the relative velocity, elasticity, etc.
				ContactType contactType = getContactType(vreln);
				if (contactType == ContactType::COLLIDING) {
					glm::vec3 term1an = A->Iinv * glm::cross(ra, contactData.normal);
					glm::vec3 term1bn = B->Iinv * glm::cross(rb, contactData.normal);
					float term2an = glm::dot(contactData.normal, glm::cross(term1an, ra));
					float term2bn = glm::dot(contactData.normal, glm::cross(term1bn, rb));
					float numerator = -(1.0f + elasticity) * vreln;
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

					// Keep looping to apply impulses until there are no more colliding contacts
					hadCollidingContact = true;
				}
			}
		}

		Logger::logIf(logContacts, Logger::LGL_INFO, "Colliding contact eliminated in {} iterations\n\n", iterCount);
	}
	void CollisionHandler::reclassifyContacts(utl::svec<SceneObject>& sceneObjects) {
		Logger::logIf(logContacts, Logger::LGL_INFO, "Reclassifying objects based on {} contacts:\n", currentContacts.size());

		// Clear the restingContacts vector and reclassify each contact after impulses were applied during colliding contact elimination.
		restingContacts.clear();
		for (const auto& contact : currentContacts) {
			utl::str colliderName = contact.get<2>()->getName();
			utl::str collideeName = contact.get<3>()->getName();
			float vreln = calculateRelativeVelocityOf(contact);
			ContactType contactType = getContactType(vreln);

			if (contactType == ContactType::COLLIDING) {
				Logger::logIf(logContacts, Logger::LGL_WARN, "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
					contact.get<0>(), colliderName, collideeName, contactTypeToString(contactType), vreln);
			}

			if (contactType == ContactType::SEPARATING) {
				Logger::logIf(logContacts, Logger::LGL_INFO, "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
					contact.get<0>(), colliderName, collideeName, contactTypeToString(contactType), vreln);
			}

			if (contactType == ContactType::RESTING) {
				Logger::logIf(logContacts, Logger::LGL_INFO, "Contact{}: collider={}, collidee={}, type={}, vreln={:.6f}\n",
					contact.get<0>(), colliderName, collideeName, contactTypeToString(contactType), vreln);

				restingContacts.push_back(contact);
			}
		}
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	// Resting contact resolution
	void CollisionHandler::computeRestingContactMatrix(eig::matd& matrix) {
		auto computeElementAt = [](const CONTACT& ci, const CONTACT& cj) {
			SceneObject* cia = ci.get<2>();
			SceneObject* cib = ci.get<3>();
			SceneObject* cja = cj.get<2>();
			SceneObject* cjb = cj.get<3>();

			if ((cia != cja) && (cib != cjb) && (cia != cjb) && (cib != cja)) {
				return 0.0f;
			}

			Collider::ContactData contactDataI = ci.get<1>();
			Collider::ContactData contactDataJ = cj.get<1>();

			ribo::BodyData* A = &(cia->getPhysicsSolver()->Body);
			ribo::BodyData* B = &(cib->getPhysicsSolver()->Body);

			glm::vec3 ni = contactDataI.normal;
			glm::vec3 nj = contactDataJ.normal;
			glm::vec3 pi = contactDataI.point;
			glm::vec3 pj = contactDataJ.point;
			glm::vec3 ra = pi - A->X;
			glm::vec3 rb = pi - B->X;

			glm::vec3 force_on_a = glm::nullvec;
			if (cja == cia) {
				force_on_a = nj;
			}
			else if (cjb == cia) {
				force_on_a = -nj;
			}
			glm::vec3 torque_on_a = glm::cross(pj - A->X, force_on_a);

			glm::vec3 force_on_b = glm::nullvec;
			if (cja == cib) {
				force_on_b = nj;
			}
			else if (cjb == cib) {
				force_on_b = -nj;
			}
			glm::vec3 torque_on_b = glm::cross(pj - B->X, force_on_b);

			glm::vec3 a_linear = force_on_a * A->invMass;
			glm::vec3 a_angular = glm::cross(A->Iinv * torque_on_a, ra);

			glm::vec3 b_linear = force_on_b * B->invMass;
			glm::vec3 b_angular = glm::cross(B->Iinv * torque_on_b, rb);

			return glm::dot(ni, a_linear + a_angular - b_linear - b_angular);
			};

		int n = static_cast<utl::uint>(restingContacts.size());
		matrix.resize(n, n);

		for (utl::uint i = 0; i < n; ++i) {
			for (utl::uint j = 0; j < n; ++j) {
				float element = computeElementAt(restingContacts[i], restingContacts[j]);
				eig::setFromFloatAt(matrix, i, j, element);
			}
		}
	}
	void CollisionHandler::computeRestingContactVector(eig::vecd& vector) {
		int n = static_cast<int>(restingContacts.size());
		vector.resize(n);
		int idx = 0;
		for (const auto& contact : restingContacts) {
			utl::uint contactIndex = contact.get<0>();
			const auto& contactData = contact.get<1>();
			SceneObject* colliderObject = contact.get<2>();
			SceneObject* collideeObject = contact.get<3>();

			ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->Body);
			ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->Body);

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
			}
			else {
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
	void CollisionHandler::resolveRestingContacts() {
		// If there are resting contacts, we need to solve for the contact forces that will prevent interpenetration and 
		// allow the objects to rest on each other without sinking or bouncing.

		Logger::setLogMode(Logger::CONTACT_LOGS);

		if (!restingContacts.empty()) {
			// Compute the contact matrix for the resting contacts
			eig::matd A;
			computeRestingContactMatrix(A);
			Logger::logIf(logContacts, Logger::LGL_INFO, "Resting contact matrix A:\n");
			eig::logMatrix(A, logContacts);

			// Compute the contact vector b for the resting contacts
			eig::vecd b;
			computeRestingContactVector(b);
			Logger::logIf(logContacts, Logger::LGL_INFO, "Resting contact vector b:\n");
			eig::logVector(b, logContacts);

			// Check if the contact matrix A is positive semi-definite, which is a requirement for solving the quadratic program for contact forces.
			if (eig::isPSD(A, PSDTolerance)) {
				Logger::logIf(logContacts, Logger::LGL_OK, "Resting contact matrix is positive semi-definite, can solve QP :)\n");

				eig::vecd forces;
				if (qp::solveBaraffContactForces(A, b, forces)) {
					Logger::logIf(logContacts, Logger::LGL_INFO, "Contact forces solved:\n");
					eig::logVector(forces, logContacts);

					for (int i = 0; i < restingContacts.size(); ++i) {
						// Extract contact information from the CONTACT tuple.
						CONTACT& contact = restingContacts[i];
						const auto& contactData = contact.get<1>();
						SceneObject* colliderObject = contact.get<2>();
						SceneObject* collideeObject = contact.get<3>();

						// Track the maximum resting force applied for logging purposes.
						float forceMagnitude = eig::getAsFloatAt(forces, i);
						if (forceMagnitude > maxRestingForce) {
							Logger::logIf(logContacts, Logger::LGL_WARN, "Resting force calculation diverged or too large\n");
							continue;
						}

						// Apply the contact force to both the collider and collidee objects in opposite directions along the contact normal.
						glm::vec3 force = forceMagnitude * contactData.normal;
						colliderObject->getPhysicsSolver()->Body.force += force;
						colliderObject->getPhysicsSolver()->Body.torque += glm::cross(contactData.point - colliderObject->getPhysicsSolver()->Body.X, force);
						collideeObject->getPhysicsSolver()->Body.force -= force;
						collideeObject->getPhysicsSolver()->Body.torque -= glm::cross(contactData.point - collideeObject->getPhysicsSolver()->Body.X, force);
					}
				}
			}
			else {
				Logger::logIf(logContacts, Logger::LGL_WARN, "Resting contact matrix is not positive semi-definite, cannot solve QP\n");
			}
		}

		Logger::logIf(logContacts, Logger::LGL_EMPTY, "\n-------------------------------------------------------------------\n\n");
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	// Interpenetration resolution
	void CollisionHandler::resolveInterpenetrations(utl::svec<SceneObject>& sceneObjects) {
		utl::vec<CONTACT> contacts;
		resolveCollisions(contacts, sceneObjects);

		utl::uvmap<utl::ull, utl::vec<CONTACT>> contactsPerPair;
		utl::uvmap<utl::ull, utl::tup<SceneObject*, SceneObject*>> objectsPerPair;

		for (const auto& contact : contacts) {
			utl::ull pairHash = utl::makePointerPairHash(contact.get<2>(), contact.get<3>());
			contactsPerPair[pairHash].push_back(contact);
			objectsPerPair[pairHash] = { contact.get<2>(), contact.get<3>() };
		}

		for (const auto& [pairHash, pairContacts] : contactsPerPair) {
			auto object1 = objectsPerPair[pairHash].get<0>();
			auto object2 = objectsPerPair[pairHash].get<1>();

			Collider::ContactData deepestContactData;
			float maxDist = 0.0f;
			for (const auto& contact : pairContacts) {
				Collider::ContactData contactData = contact.get<1>();
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

			glm::vec3 object1Pos = object1->getPhysicsSolver()->Body.X;
			glm::vec3 object2Pos = object2->getPhysicsSolver()->Body.X;

			glm::vec3 point = deepestContactData.point;
			glm::vec3 normal = deepestContactData.normal;
			glm::vec3 displacement = *deepestContactData.depth;

			SceneObject* objectToDisplace;
			if (glm::dot(object1Pos - point, normal) > 0.0f) {
				objectToDisplace = object1;
			}
			else if (glm::dot(object2Pos - point, normal) > 0.0f) {
				objectToDisplace = object2;
			}
			else {
				continue;
			}

			objectToDisplace->getPhysicsSolver()->Body.X += displacement;
			objectToDisplace->updateTransformations();
		}
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	// Debug and draw
	void CollisionHandler::debugContacts(const Camera& camera) {
		utl::vec<glm::vec3> contactPoints;
		for (const auto& contact : currentContacts) {
			utl::uint contactIndex = contact.get<0>();
			Collider::ContactData contactData = contact.get<1>();
			SceneObject* colliderObject = contact.get<2>();
			SceneObject* collideeObject = contact.get<3>();

			ribo::BodyData* A = &(colliderObject->getPhysicsSolver()->Body);
			ribo::BodyData* B = &(collideeObject->getPhysicsSolver()->Body);

			contactPoints.push_back(contactData.point);
			glm::vec3 ra = contactData.point - A->X;
			glm::vec3 rb = contactData.point - B->X;
			glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
			glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
			glm::vec3 vrel = velpa - velpb;

			DebugDrawer::setOverrideZ(1);
			DebugDrawer::setMode(GL_LINES);

			float debugveclen = 1.0f;

			DebugDrawer::setVertexData({ contactData.point, contactData.point + debugveclen * velpa });
			DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 0.0f));

			DebugDrawer::setVertexData({ contactData.point, contactData.point + debugveclen * velpb });
			DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 1.0f));

			DebugDrawer::setVertexData({ contactData.point, contactData.point + debugveclen * vrel });
			DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 1.0f));

			if (drawNormals) {
				DebugDrawer::setVertexData({ contactData.point, contactData.point + debugveclen * contactData.normal });
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 1.0f, 0.0f));
			}

			if (contactData.isVertexFace) {
				glm::vec3 contactPoint1 = contactData.point;
				glm::vec3 contactPoint2 = contactPoint1 + *contactData.depth;
				DebugDrawer::setVertexData({ contactPoint1, contactPoint2 });
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 0.0f));
			}
			else {
				glm::vec3 edgePoint1 = contactData.edgeA[0];
				glm::vec3 edgePoint2 = contactData.edgeB[0];
				DebugDrawer::setVertexData({ edgePoint1, edgePoint2 });
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 0.0f));

				DebugDrawer::setMode(GL_POINTS);
				DebugDrawer::setVertexData({ edgePoint1 });
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 0.0f, 1.0f));
				DebugDrawer::setVertexData({ edgePoint2 });
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 1.0f));
			}
		}

		DebugDrawer::setMode(GL_POINTS);
		DebugDrawer::setOverrideZ(1);
		DebugDrawer::setVertexData(contactPoints);
		DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void CollisionHandler::drawCollidersOf(const utl::svec<SceneObject>& sceneObjects, const Camera& camera) {
		for (auto& sceneObject : sceneObjects) {
			auto cuboidCollider = dynamic_cast<CuboidCollider*>(sceneObject->getCollider().get());
			if (cuboidCollider != nullptr) {
				auto p = cuboidCollider->getTransData().points; 
				auto points = utl::vec<glm::vec3>(p.begin(), p.end());
				DebugDrawer::setMode(GL_POINTS);
				DebugDrawer::setVertexData(points);
				DebugDrawer::setOverrideZ(0);
				DebugDrawer::draw(camera.getV(), camera.getP(), glm::vec3(1.0f, 0.0f, 0.0f));

				utl::vec<utl::vec<glm::vec3>> sideData;
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
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	// Main entry point
	void CollisionHandler::handleCollisions(utl::svec<SceneObject>& sceneObjects) {
		if (logStatesOnce) {
			logPhysicsStateOfObjects(logStatesOnce, true, sceneObjects);
			logStatesOnce = false;
		}

		// Clear the current contacts and calculate new contacts
		currentContacts.clear();
		if (enableBisection) {
			currentContacts = calculateContactsWithBisection(sceneObjects);
		}
		else {
			resolveCollisions(currentContacts, sceneObjects);
		}

		// Handle the calculated contacts
		if (enableContactLog && currentContacts.size() > 0) {
			logContacts = true;
		}
		if (enableImpulses) {
			applyImpulses();
		}
		if(enableRestingForces) {
			reclassifyContacts(sceneObjects);
			resolveRestingContacts();
		}

		// Stepping the time left after bisection to reach the end of the fixed time step
		if (bisectedTime >= 0.0f) {
			float remainingTime = Time::s_fixedDeltaTime - bisectedTime;
			for (auto& sceneObject : sceneObjects) {
				sceneObject->stepPhysicsBy(remainingTime);
			}
			bisectedTime = -1.0f;
		}

		// After all physics calculations resolve unrealistic interpenetrations
		if (enableDepenetration) {
			resolveInterpenetrations(sceneObjects);
		}
		else if(pushApartOnce){
			resolveInterpenetrations(sceneObjects);
			pushApartOnce = false;
		}

		lgl::Logger::setLogMode(lgl::Logger::PHYSICS_LOGS);
		Logger::logIf(enablePhysicsLog, Logger::LGL_INFO, "Post-collision state of all objects:\n\n");
		logPhysicsStateOfObjects(enablePhysicsLog, true, sceneObjects);

		Logger::incrementLogCounterFor(Logger::BISECTION_LOGS);
		Logger::incrementLogCounterFor(Logger::CONTACT_LOGS);
		Logger::incrementLogCounterFor(Logger::PHYSICS_LOGS);
		logContacts = false;
	}
	void CollisionHandler::debugDrawCollisions(const utl::svec<SceneObject>& sceneObjects, const Camera& camera) {
		if (enableDebug) {
			drawCollidersOf(sceneObjects, camera);
			debugContacts(camera);
		}
	}
}