#include "SimulationInterface.hxx"

using namespace lgl;

void SimulationInterface::renderLightingPicker() {
    static float lightPowerDens[3] = { 1.0f, 1.0f, 1.0f };

    ImGui::Begin("Light picker");
    ImGui::SliderFloat3("lightPowerDens", lightPowerDens, 0.0f, 1000.0f, "%.2f", 0);
    ImGui::End();

    m_simulationScene->getSun()->getLight()->powerDensity = glm::vec3(lightPowerDens[0], lightPowerDens[1], lightPowerDens[2]);
}

void SimulationInterface::renderSimulationSettings() {
    static float simulationSpeed = 0.005f;
    static float elasticity = 0.5f;
    static float slipperiness = 0.5f;
    static utl::cstr precision = "%.3f";

    ImGui::Begin("Simulation settings");

    bool ed = CollisionHandler::enableDebug;
    utl::cstr debugText = ed ? "Disable Debug" : "Enable Debug";
    if (ImGui::Button(debugText)) {
        CollisionHandler::enableDebug = !ed;
    }

    bool edisp = CollisionHandler::enableDisplacement;
    utl::cstr displacementText = edisp ? "Disable displacement" : "Enable displacement";
    if (ImGui::Button(displacementText)) {
        CollisionHandler::enableDisplacement = !edisp;
    }

    bool eimp = CollisionHandler::enableImpulses;
    utl::cstr impulsesText = eimp ? "Disable impulses" : "Enable impulses";
    if (ImGui::Button(impulsesText)) {
        CollisionHandler::enableImpulses = !eimp;
    }

    bool dn = CollisionHandler::draw_normals;
    utl::cstr normals_text = dn ? "Hide normals" : "Draw normals";
    if (ImGui::Button(normals_text)) {
        CollisionHandler::draw_normals = !dn;
    }

    if (ImGui::Button("Rollback simulation")) {
        m_simulationScene->rollbackSimulation();
    }

    if (ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.005f, 0.05f, precision, 0)) {
        m_simulationScene->setSimulationSpeed(simulationSpeed);
    }
    if (ImGui::SliderFloat("Elasticity", &elasticity, 0.0f, 1.0f, precision, 0)) {
        CollisionHandler::elasticity = elasticity;
    }
    if (ImGui::SliderFloat("Slipperiness", &slipperiness, 0.0f, 1.0f, precision, 0)) {
        CollisionHandler::slipperiness = slipperiness;
    }

	ImGui::End();   
}

void SimulationInterface::renderTerrainSettings() {
    static float amplitude = 0.0f;
    static float frequency = 0.0f;
    static float phase = 0.0f;
    static int iterations = 0;
    static utl::cstr precision = "%.2f";

    if (m_simulationScene->getTerrain().get() == nullptr) {
        return;
    }

    ImGui::Begin("Terrain settings");

    ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 1.0f, precision, 0);
    ImGui::SliderFloat("frequency", &frequency, 0.0f, 50.0f, precision, 0);
    ImGui::SliderFloat("phase", &phase, 0.0f, 25.0f, precision, 0);
    ImGui::SliderInt("iterations", &iterations, 1, 32);

    if (ImGui::Button("Reset to default")) {
        amplitude = 0.0f; frequency = 0.0f; phase = 0.1f; iterations = 3;
    }
    ImGui::End();

    m_simulationScene->getTerrain()->setParameters(amplitude, frequency, phase, iterations);
}

void SimulationInterface::renderAdditionalUI() {
	renderLightingPicker();
	renderSimulationSettings();
	renderTerrainSettings();
}

SimulationInterface::SimulationInterface(GLFWwindow* window, utl::sptr<Scene> scene) 
	: UserInterface(window, scene) {
    
	m_simulationScene = utl::sptrCast<SimulationScene>(scene);
    if (!m_simulationScene) {
		throw utl::except("'SimulationInterface' can only by initialized with 'SimulationScene' instance");
    }
}

