#include "Sphere.hpp"

using namespace lgl;

Sphere::Sphere(float radius) : SceneObject() {
	m_radius = radius;

	m_mesh = std::make_shared<SphereMesh>(30, 30, radius);
	m_collider = std::make_shared<SphereCollider>(radius);

	float inverseMass = 0.01f;
	float inertiaScalar = 2.0f / 5.0f * (1.0f / inverseMass) * radius * radius;
	glm::mat3 Ibody = glm::mat3(
		glm::vec3(inertiaScalar, 0.0f, 0.0f), 
		glm::vec3(0.0f, inertiaScalar, 0.0f), 
		glm::vec3(0.0f, 0.0f, inertiaScalar)
	);
	glm::mat3 Ibodyinv = glm::inverse(Ibody);

	auto body = ribo::BodyData{
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

		glm::vec3(0.0f, -1.0f, 0.0f),        // force
		glm::vec3(0.0f)         // torque
	};
	m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(body);

	m_transformation = std::make_shared<Transformation>();
	m_mesh->setTransformation(m_transformation);
	m_mesh->create();
	m_collider->setTransformation(m_transformation.get());
}

void Sphere::stepPhysicsBy(float dt) {
	glm::vec3 c = dynamic_cast<SphereCollider*>(m_collider.get())->getTransCenter();
	glm::vec3 v = c + m_radius * glm::vec3(0.0f, 0.0f, 1.0f);
	m_physicsSolver->computeTotalTorque({ v });
	m_physicsSolver->updateState(dt);
}