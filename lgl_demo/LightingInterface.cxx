#include "LightingInterface.hxx"

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

LightingInterface::LightingInterface(GLFWwindow* window, utl::sptr<Scene> scene)
	: UserInterface(window, scene) {
	
	for (auto sceneObject : scene->getSceneObjects()) {
		auto sun = utl::sptrCast<Sun>(sceneObject);
		if (sun.get() != nullptr) {
			m_sun = sun;
			break;
		}
		else {
			throw utl::except("LightingInterface initialization error: scene has no light source\n");
		}
	}
}