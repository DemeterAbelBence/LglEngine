#include "SceneObject.hxx"

namespace lgl {
	SceneObject::SceneObject() {}

	SceneObject::SceneObject(float size, utl::sptr<ModelMesh> mesh) {
		m_mesh = mesh;
		m_collider = utl::makeSptr<NullCollider>();

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
		glm::vec3 scaling = glm::vec3(size, size, size);
		m_transformation->setScaleMatrix(scaling);
		m_collider->setTransformation(m_transformation.get());

		m_mesh->setTransformation(m_transformation);
	}

	void SceneObject::setProgram(utl::sptr<GpuProgram> program) {
		if (m_mesh) {
			m_mesh->setProgram(program);
		}
		else {
			Logger::log(Logger::LGL_WARN, "No mesh to set program for SceneObject {}\n", m_name);
		}
	}

	void SceneObject::setMaterial(utl::sptr<Material> material) {
		if (m_mesh) {
			m_mesh->setMaterial(material);
		}
		else {
			Logger::log(Logger::LGL_WARN, "No mesh to set material for SceneObject {}\n", m_name);
		}
	}

	void SceneObject::setTextures(const utl::vec<utl::sptr<Texture>>& textures) {
		if (m_mesh) {
			for (const auto& texture : textures) {
				m_mesh->addTexture(texture);
			}
		}
		else {
			Logger::log(Logger::LGL_WARN, "No mesh to set textures for SceneObject {}\n", m_name);
		}
	}

	void SceneObject::translate(glm::vec3 translation) {
		m_physicsSolver->Body.X += translation;
		updateTransformations();
	}

	void SceneObject::scale(glm::vec3 scale) {
		m_transformation->setScaleMatrix(scale);
		updateTransformations();
		updateRigidBody();
	}

	void SceneObject::setRotation(glm::mat4 R) {
		m_physicsSolver->Body.R = glm::mat3(R);
		updateTransformations();
	}

	void SceneObject::updateTransformations() {
		m_transformation->setTranslationMarix(m_physicsSolver->Body.X);

		m_transformation->setR(glm::mat4(
			glm::vec4(m_physicsSolver->Body.R[0], 0.0f),
			glm::vec4(m_physicsSolver->Body.R[1], 0.0f),
			glm::vec4(m_physicsSolver->Body.R[2], 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		));

		m_collider->updateTransformations();
	}

	ribo::BodyData SceneObject::initializePhysics() {
		return ribo::BodyData{};
	}

	void SceneObject::updateRigidBody() {
	}

	void SceneObject::stepPhysicsBy(float dt) {
		m_physicsSolver->updateState(dt);
		updateTransformations();
	}

	void SceneObject::resetBodyState() {
		m_physicsSolver->rollbackToInitial();
		updateTransformations();
	}

	void SceneObject::draw(const Camera& camera) const {
		if (m_mesh) {
			m_mesh->draw(camera);
		}
		else {
			Logger::log(Logger::LGL_WARN, "No mesh to draw for SceneObject {}\n", m_name);
		}
	}

	void SceneObject::handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L) {
		auto program = m_mesh->getProgram();
		if (program) {
			program->bind();
			program->setVec3("light.position", light.position);
			program->setVec3("light.powerDensity", light.powerDensity);
			program->setMat4("L", L);
			depthMap.bindTexture(*program);
		}
		else {
			Logger::log(Logger::LGL_WARN, "No program to handle lighting for SceneObject {}\n", m_name);
		}
	}
}