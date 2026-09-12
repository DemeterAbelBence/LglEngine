#include "Box.hxx"

namespace lgl {
	std::shared_ptr<SceneObject> Box::clone() {
		auto cuboidCollider = std::dynamic_pointer_cast<CuboidCollider>(m_collider);

		std::shared_ptr<Box> other = std::make_shared<Box>();
		other->m_mesh = m_mesh;
		other->m_transformation = std::make_shared<Transformation>(*m_transformation);
		other->m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(*m_physicsSolver);
		other->m_collider = std::make_shared<CuboidCollider>(*cuboidCollider);
		other->m_isStationary = m_isStationary;

		return other;
	}

	ribo::BodyData Box::initializePhysics() {
		float inverseMass;
		glm::mat3 Ibody;
		glm::mat3 Ibodyinv;
		glm::vec3 force;

		if (!m_isStationary) {
			inverseMass = 1.0f / m_massFactor;
			Ibody = 1.0f / (12.0f * inverseMass) * glm::mat3(
				glm::vec3(2.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 2.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 2.0f)
			);

			Ibodyinv = glm::inverse(Ibody);
			force = glm::vec3(0.0f, -1.0f, 0.0f) / inverseMass;
		}
		else {
			inverseMass = 0.0f;
			Ibody = glm::mat3(0.0f);
			Ibodyinv = glm::mat3(0.0f);
			force = glm::vec3(0.0f, -9.81f, 0.0f);
		}

		auto bodyData = ribo::BodyData{
			inverseMass,			// invMass
			Ibody,                  // Ibody
			Ibodyinv,               // Ibodyinv

			glm::vec3(0.0f),        // X 
			glm::mat3(1.0f),        // R 
			glm::vec3(0.0f),        // P
			glm::vec3(0.0f),        // L 

			glm::mat3(1.0f),        // Iinv 
			glm::vec3(0.0f),        // vel 
			glm::vec3(0.0f),        // omega 

			force,                               // force
			glm::vec3(0.0f)                      // torque
		};

		return bodyData;
	}

	Box::Box() : SceneObject() {}

	Box::Box(bool isStationary, std::shared_ptr<ModelMesh> modelMesh) {
		m_isStationary = isStationary;

		auto bodyData = initializePhysics();
		m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(bodyData);

		m_mesh = modelMesh;
		m_collider = std::make_shared<CuboidCollider>();

		m_transformation = std::make_shared<Transformation>();
		m_mesh->setTransformation(m_transformation);
		m_collider->setTransformation(m_transformation);
	}

	Box::Box(bool isStationary) : SceneObject() {
		m_isStationary = isStationary;

		auto bodyData = initializePhysics();
		m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(bodyData);

		m_mesh = std::make_shared<CuboidMesh>();
		m_collider = std::make_shared<CuboidCollider>();

		m_transformation = std::make_shared<Transformation>();
		m_mesh->setTransformation(m_transformation);
		m_collider->setTransformation(m_transformation);
	}

	void Box::updateRigidBody() {
		glm::vec3 scaling = m_transformation->getScaling();
		float w = scaling.x;
		float h = scaling.y;
		float l = scaling.z;

		if (!m_isStationary) {
			float& invMass = m_physicsSolver->Body.invMass;
			invMass = 1.0f / (w * h * l * m_massFactor);

			glm::mat3& I = m_physicsSolver->Body.Ibody;
			I = 1.0f / (12.0f * invMass) * glm::mat3(
				glm::vec3(h * h + l * l, 0.0f, 0.0f),
				glm::vec3(0.0f, w * w + l * l, 0.0f),
				glm::vec3(0.0f, 0.0f, w * w + h * h)
			);

			glm::mat3& Iinv = m_physicsSolver->Body.Ibodyinv;
			Iinv = glm::inverse(I);

			glm::vec3& force = m_physicsSolver->Body.force;
			force = glm::vec3(0.0f, -9.81f, 0.0f) / invMass;
		}
	}

	void Box::draw(const Camera& camera) const {
		m_mesh->setTransformation(m_transformation);
		m_mesh->draw(camera);
	}
}