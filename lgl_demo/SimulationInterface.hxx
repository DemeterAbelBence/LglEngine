#pragma

import utl;
import glm;

#include <eng/UserInterface.hxx>
#include <eng/Scene.hxx>

#include "SimulationScene.hxx"

class SimulationInterface : public lgl::UserInterface {
private:
	std::shared_ptr<SimulationScene> m_simulationScene;

    inline static const ImVec4 m_blueBtn = ImVec4(0.00f, 0.45f, 0.70f, 1.00f);
    inline static const ImVec4 m_blueBtnHov = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
    inline static const ImVec4 m_blueBtnAct = ImVec4(0.00f, 0.35f, 0.55f, 1.00f);
    inline static const ImVec4 m_redBtn = ImVec4(0.70f, 0.15f, 0.15f, 1.00f);
    inline static const ImVec4 m_redBtnHov = ImVec4(0.90f, 0.20f, 0.20f, 1.00f);
    inline static const ImVec4 m_redBtnAct = ImVec4(0.60f, 0.10f, 0.10f, 1.00f);
    inline static const ImVec4 m_greenBtn = ImVec4(0.15f, 0.60f, 0.20f, 1.00f);
    inline static const ImVec4 m_greenBtnHov = ImVec4(0.20f, 0.75f, 0.25f, 1.00f);
    inline static const ImVec4 m_greenBtnAct = ImVec4(0.10f, 0.50f, 0.16f, 1.00f);

    inline static float m_buttonWidth = 175.0f;

private:
	void renderLightingPicker();
	void renderSimulationSettings();
	void renderTerrainSettings();

protected:
	void renderAdditionalUI() override;

public:
	SimulationInterface(GLFWwindow* window, std::shared_ptr<lgl::Scene> scene);
};
