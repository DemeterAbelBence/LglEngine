#include "Sun.hxx"

namespace lgl {
	Sun::Sun(float radius, glm::vec3 powerDensity) : SceneObject() {
		m_mesh = utl::makeSptr<SphereMesh>(30, 30, radius);
		m_collider = utl::makeSptr<NullCollider>();
		m_light = utl::makeUptr<Light>(Light{
			powerDensity,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)),
			true
			});

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
		m_physicsSolver = utl::makeSptr<ribo::PhysicsSolver>(body);

		m_transformation = utl::makeSptr<Transformation>();
		m_mesh->setTransformation(m_transformation);
		m_mesh->create();
		m_collider->setTransformation(m_transformation.get());
	}

	void Sun::directPerspectiveOf(Camera& camera) {
		m_light->position = m_physicsSolver->Body.X;
		glm::mat4 lightProj = makeLightProjection(0.001f, 100.0f);
		glm::mat4 lightView = makeLightView(m_light->position, -m_light->direction);
		camera.setProjection(lightProj);
		camera.setView(lightView);
		m_lightMatrix = lightView * lightProj;
	}

	void Sun::lightUpScene(const utl::svec<SceneObject>& sceneObjects, const FrameBuffer& depthMap) {
		m_light->position = m_physicsSolver->Body.X;
		for (const auto& s : sceneObjects) {
			s->handleLighting(*m_light, depthMap, m_lightMatrix);
		}
	}
}