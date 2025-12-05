#include "Scene.hpp"

using namespace lgl;

void Scene::initCameraEvents() {
	using namespace keyin;

	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_S), &Camera::moveUp, -1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_W), &Camera::moveUp, 1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_A), &Camera::moveRight, -1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_D), &Camera::moveRight, 1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_LEFT_SHIFT), &Camera::moveForward, -1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera, int>(Key(GLFW_KEY_LEFT_CONTROL), &Camera::moveForward, 1, false));
	m_cameraEventHandler
		.addEvent(KeyEvent<Camera>(Key(GLFW_KEY_LEFT_ALT), &Camera::toggleCaptureMode, true));
}

void Scene::initObjectEvents() {
	using namespace keyin;

	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_W), &SceneObject::translate, glm::vec3(0.0f, 1.0f, 0.0f), false));
	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_S), &SceneObject::translate, glm::vec3(0.0f, -1.0f, 0.0f), false));
	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_D), &SceneObject::translate, glm::vec3(1.0f, 0.0f, 0.0f), false));
	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_A), &SceneObject::translate, glm::vec3(-1.0f, 0.0f, 0.0f), false));
	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_LEFT_SHIFT), &SceneObject::translate, glm::vec3(0.0f, 0.0f, -1.0f), false));
	m_objectEventHandler.addEvent(KeyEvent<SceneObject, glm::vec3>(
		Key(GLFW_KEY_LEFT_CONTROL), &SceneObject::translate, glm::vec3(0.0f, 0.0f, 1.0f), false));
}

Scene::Scene() {
	m_objectIndex = 0;
	m_moveCamera = true;

	m_camera = std::make_unique<Camera>();

	initCameraEvents();
	initObjectEvents();
}

void Scene::getNextObject() {
	if (++m_objectIndex == m_sceneObjects.size()) {
		m_objectIndex = 0;
	}
}

void Scene::setPrimitiveType(UINT type) {
	for (const auto& s : m_sceneObjects) {
		auto& mesh = s->getMesh();
		if (mesh != nullptr) {
			mesh->setPrimitive(type);
		}
	}
}

void Scene::setCameraMoveSpeed(float speed) {
	m_camera->setMoveSpeed(speed);
}

void Scene::setObjectMoveSpeed(float speed) {
	for (auto& event : m_objectEventHandler.getArgEvents()) {
		glm::vec3& arg = event.getArgRef();
		arg.x = arg.x < 0.0f ? -speed : arg.x;
		arg.y = arg.y < 0.0f ? -speed : arg.y;
		arg.z = arg.z < 0.0f ? -speed : arg.z;

		arg.x = arg.x > 0.0f ? speed : arg.x;
		arg.y = arg.y > 0.0f ? speed : arg.y;
		arg.z = arg.z > 0.0f ? speed : arg.z;
	}
}
