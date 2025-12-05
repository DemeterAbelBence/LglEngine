#pragma

#include <gl/BaseDefs.hpp>

#include <eng/UserInterface.hpp>
#include <eng/Scene.hpp>

#include "SimulationScene.hpp"

using namespace lgl;

class SimulationInterface : public UserInterface {
private:
	SPTR<SimulationScene> m_simulationScene;

private:
	void renderLightingPicker();
	void renderSimulationSettings();
	void renderTerrainSettings();

protected:
	void renderAdditionalUI() override;

public:
	SimulationInterface(GLFWwindow* window, SPTR<Scene> scene);
};
