#include "SceneObject.hpp"

using namespace lgl;

SceneObject::SceneObject() {}

void SceneObject::setProgram(SPTR<GpuProgram> program) {
	if (m_mesh) {
		m_mesh->setProgram(program);
	}
	else {
		std::cout << "No mesh to set program for SceneObject " << m_name << "\n";
	}
}

void SceneObject::setMaterial(SPTR<Material> material) {
	if (m_mesh) {
		m_mesh->setMaterial(material);
	}
	else {
		std::cout << "No mesh to set material for SceneObject " << m_name << "\n";
	}
}

void SceneObject::setTextures(const std::vector<SPTR<Texture>>& textures) {
	if (m_mesh) {
		for (const auto& texture : textures) {
			m_mesh->addTexture(texture);
		}
	}
	else {
		std::cout << "No mesh to set textures for SceneObject " << m_name << "\n";
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
		std::cout << "No mesh to draw for SceneObject " << m_name << "\n";
	}
}

void SceneObject::handleLighting(const Light& light, const FrameBuffer& depthMap, const glm::mat4& L) {
	auto program = m_mesh->getProgram();
	if (program) {
		program->bind();
		program->setVec3(std::string("light.position"), light.position);
		program->setVec3(std::string("light.powerDensity"), light.powerDensity);
		program->setMat4(std::string("L"), L);
		depthMap.bindTexture(*program);
	}
	else {
		std::cout << "No program to handle lighting for SceneObject " << m_name << "\n";
	}
}
