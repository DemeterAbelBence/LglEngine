#include "LightingInterface.hpp"

using namespace lgl;

void LightingInterface::renderLightingPicker() {
	static float lightPowerDens[3] = { 1.0f, 1.0f, 1.0f };
	ImGui::Begin("Light picker");
	ImGui::SliderFloat3("lightPowerDens", lightPowerDens, 0.0f, 1000.0f, "%.2f", 0);
	ImGui::End();
	m_sun->getLight()->powerDensity = glm::vec3(lightPowerDens[0], lightPowerDens[1], lightPowerDens[2]);
}

void LightingInterface::renderAdditionalUI() {
	renderLightingPicker();
}

LightingInterface::LightingInterface(GLFWwindow* window, SPTR<Scene> scene)
	: UserInterface(window, scene) {
	
	for (auto sceneObject : scene->getSceneObjects()) {
		auto sun = std::dynamic_pointer_cast<Sun>(sceneObject);
		if (sun != nullptr) {
			m_sun = sun;
			break;
		}
		else {
			throw std::exception("LightingInterface initialization error: scene has no light source\n");
		}
	}
}