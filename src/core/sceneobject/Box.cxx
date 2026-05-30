#include "Box.hxx"

namespace lgl {
	utl::sptr<SceneObject> Box::clone() {
		utl::sptr<Box> other = utl::makeSptr<Box>();

		other->m_mesh = m_mesh;
		other->m_transformation = utl::makeSptr<Transformation>(*m_transformation);
		other->m_physicsSolver = utl::makeSptr<ribo::PhysicsSolver>(*m_physicsSolver);

		auto cuboidCollider = utl::sptrCast<CuboidCollider>(m_collider);
		other->m_collider = utl::makeSptr<CuboidCollider>(*cuboidCollider);

		other->m_dimensions = m_dimensions;
		other->m_isStationary = m_isStationary;

		return other;
	}

	ribo::BodyData Box::initializePhysics() {
		float inverseMass;
		glm::mat3 Ibody;
		glm::mat3 Ibodyinv;
		glm::vec3 force;

		float w = m_dimensions.x;
		float h = m_dimensions.y;
		float l = m_dimensions.z;

		if (!m_isStationary) {
			inverseMass = 1.0f / (w * h * l * m_massFactor);
			Ibody = 1.0f / (12.0f * inverseMass) * glm::mat3(
				glm::vec3(h * h + l * l, 0.0f, 0.0f),
				glm::vec3(0.0f, w * w + l * l, 0.0f),
				glm::vec3(0.0f, 0.0f, w * w + h * h)
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

	Box::Box(bool isStationary, utl::sptr<ModelMesh> modelMesh) {
		m_isStationary = isStationary;
		m_dimensions = glm::vec3(1.0f, 1.0f, 1.0f);

		auto bodyData = initializePhysics();
		m_physicsSolver = utl::makeSptr<ribo::PhysicsSolver>(bodyData);

		m_mesh = modelMesh;
		m_mesh->create();
		m_collider = utl::makeSptr<CuboidCollider>(m_dimensions);

		m_transformation = utl::makeSptr<Transformation>();
		m_mesh->setTransformation(m_transformation);
		m_collider->setTransformation(m_transformation);
	}

	Box::Box(bool isStationary, const glm::vec3& dimensions) : SceneObject() {
		m_dimensions = dimensions;
		m_isStationary = isStationary;

		auto bodyData = initializePhysics();
		m_physicsSolver = utl::makeSptr<ribo::PhysicsSolver>(bodyData);

		m_mesh = utl::makeSptr<CuboidMesh>(m_dimensions);
		m_mesh->create();
		m_collider = utl::makeSptr<CuboidCollider>(m_dimensions);

		m_transformation = utl::makeSptr<Transformation>();
		m_mesh->setTransformation(m_transformation);
		m_collider->setTransformation(m_transformation);
	}

	void Box::updateRigidBody() {
		glm::vec3 scaling = m_transformation->getScaling();
		float w = m_dimensions.x * scaling.x;
		float h = m_dimensions.y * scaling.y;
		float l = m_dimensions.z * scaling.z;

		m_dimensions = glm::vec3(w, h, l);

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

	void Box::draw(const Camera& camera) const {
		m_mesh->setTransformation(m_transformation);
		m_mesh->draw(camera);
	}
}