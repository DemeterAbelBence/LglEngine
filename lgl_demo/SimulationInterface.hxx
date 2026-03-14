#pragma

import lglmodule;
import glmmodule;

#include <eng/UserInterface.hxx>
#include <eng/Scene.hxx>

#include "SimulationScene.hxx"

class SimulationInterface : public lgl::UserInterface {
private:
	lgl::utl::sptr<SimulationScene> m_simulationScene;

private:
	void renderLightingPicker();
	void renderSimulationSettings();
	void renderTerrainSettings();

protected:
	void renderAdditionalUI() override;

public:
	SimulationInterface(GLFWwindow* window, lgl::utl::sptr<lgl::Scene> scene);
};
