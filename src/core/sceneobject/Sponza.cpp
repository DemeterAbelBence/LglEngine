#include "Sponza.hpp"

using namespace lgl;

Sponza::Sponza(float size, SPTR<ModelMesh> mesh) {
	m_mesh = mesh;
	m_collider = std::make_shared<NullCollider>();

	auto body = ribo::BodyData{
		0.0f,			        // invMass
		glm::mat3(1.0f),        // Ibody
		glm::mat3(1.0f),        // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P 
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f),        // force
		glm::vec3(0.0f)         // torque
	};
	m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(body);

	m_transformation = std::make_shared<Transformation>();
	glm::vec3 scaling = glm::vec3(size, size, size);
	m_transformation->setScaleMatrix(scaling);
	m_collider->setTransformation(m_transformation.get());

	m_mesh->setTransformation(m_transformation);
}


