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
    static const char* precision = "%.3f";

    ImGui::Begin("Simulation settings");

    // onToggle is a plain function pointer (no new std symbols)
    auto toggleButton = [&](bool& flag, const char* onText, const char* offText, void (*onToggle)(bool) = nullptr) {
        bool current = flag;
        const char* text = current ? onText : offText;
        // Switch mapping: true -> red, false -> green
        const ImVec4& col = current ? m_redBtn : m_greenBtn;
        const ImVec4& colHov = current ? m_redBtnHov : m_greenBtnHov;
        const ImVec4& colAct = current ? m_redBtnAct : m_greenBtnAct;
        ImGui::PushStyleColor(ImGuiCol_Button, col);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colHov);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, colAct);
        if (ImGui::Button(text, ImVec2(m_buttonWidth, 0))) {
            flag = !current;
            if (onToggle) onToggle(flag);
        }
        ImGui::PopStyleColor(3);
    };

    ImGui::Columns(2, "toggle_columns", false);

    toggleButton(CollisionHandler::enableDebug, "Disable Debug", "Enable Debug");

    ImGui::NextColumn();

    toggleButton(CollisionHandler::enableBisection, "Disable bisection", "Enable bisection");

    ImGui::NextColumn();

    toggleButton(CollisionHandler::enableImpulses, "Disable impulses", "Enable impulses");

    ImGui::NextColumn();

    toggleButton(CollisionHandler::enableRestingForces, "Disable resting forces", "Enable resting forces");

    ImGui::NextColumn();

    toggleButton(CollisionHandler::enableDepenetration, "Disable depenetration", "Enable depenetration");

    toggleButton(CollisionHandler::drawVectors, "Hide vectors", "Draw vectors");

    toggleButton(m_simulationScene->s_randomizeRollback, "Disable random rollback", "Enable random rollback");

    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
    if (ImGui::Button("Push apart once", ImVec2(m_buttonWidth, 0))) { 
        CollisionHandler::pushApartOnce = true; 
        m_simulationScene->handleCollisions();
    }
    if (ImGui::Button("Log states once", ImVec2(m_buttonWidth, 0))) { 
        CollisionHandler::logStatesOnce = true; 
    }
    ImGui::PopStyleColor(3);

    toggleButton(m_simulationScene->s_lockSimulation, "Unlock simulation", "Lock simulation");

    ImGui::Columns(1);

    ImGui::Separator();

    // File logging toggle (use toggleButton to get color + optional onToggle)
    // Use a plain function pointer initialized from a captureless lambda
    void (*fileLogToggleFunc)(bool) = [](bool enabled) {
        if (enabled) utl::setLogMode(utl::GENERAL_LOGS);
    };
    toggleButton(utl::logToFile, "Disable file logging", "Enable file logging", fileLogToggleFunc);

    // Contact log toggle
    toggleButton(CollisionHandler::enableContactLog, "Disable contact log", "Enable contact log", nullptr);

    // Bisection log toggle
    toggleButton(CollisionHandler::enableBisectionLog, "Disable bisection log", "Enable bisection log", nullptr);

    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
    if (ImGui::Button("Simulate forward", ImVec2(m_buttonWidth, 0))) { m_simulationScene->advanceSimulation(Time::s_fixedDeltaTime); }
    if (ImGui::Button("Simulate backward", ImVec2(m_buttonWidth, 0))) { m_simulationScene->advanceSimulation(-Time::s_fixedDeltaTime); }
    if (ImGui::Button("Rollback to initial", ImVec2(m_buttonWidth, 0))) { m_simulationScene->rollbackToinitial(); }
    if (ImGui::Button("Rollback to previous", ImVec2(m_buttonWidth, 0))) { m_simulationScene->rollbackToPrevious(); }
    ImGui::PopStyleColor(3);

    ImGui::Separator();

    if (ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.001f, 0.05f, precision, 0)) {
		Time::s_fixedDeltaTime = simulationSpeed;
        m_simulationScene->setSimulationSpeed(simulationSpeed);
    }

    ImGui::SliderFloat("Elasticity", &CollisionHandler::elasticity, 0.0f, 1.0f, precision, 0);
    ImGui::SliderFloat("Bisection Bias", &CollisionHandler::bisectionBias, 0.0f, 0.01f, "%.5f", 0);
    ImGui::SliderFloat("Contact Bias", &CollisionHandler::contactBias, 0.0f, 0.01f, "%.5f", 0);
	ImGui::SliderFloat("PSD Tolerance", &CollisionHandler::PSDTolerance, 0.0f, 0.001f, "%.7f", 0);
    ImGui::SliderInt("logFrequency", reinterpret_cast<int*>(&utl::logFrequency), 1, 32);

    ImGui::Separator();

    static char sceneSaveName[128] = "";
    ImGui::InputText("Scene Save Name", sceneSaveName, IM_ARRAYSIZE(sceneSaveName));
    
    ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
    if (ImGui::Button("Save Scene", ImVec2(m_buttonWidth, 0))) {
        std::string savePath = std::format("{}/{}", ribo::baseSerializationPath, sceneSaveName);
        m_simulationScene->saveObjectStates(savePath);
    }

    if (ImGui::Button("Load Scene", ImVec2(m_buttonWidth, 0))) {
        std::string savePath = std::format("{}/{}", ribo::baseSerializationPath, sceneSaveName);
        m_simulationScene->loadObjectStates(savePath);
    }
    ImGui::PopStyleColor(3);

	ImGui::End();   
}

void SimulationInterface::renderTerrainSettings() {
    static float amplitude = 0.0f;
    static float frequency = 0.0f;
    static float phase = 0.0f;
    static int iterations = 0;
    static const char* precision = "%.2f";

    if (m_simulationScene->getTerrain().get() == nullptr) {
        return;
    }

    ImGui::Begin("Terrain settings");

    ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 1.0f, precision, 0);
    ImGui::SliderFloat("frequency", &frequency, 0.0f, 50.0f, precision, 0);
    ImGui::SliderFloat("phase", &phase, 0.0f, 25.0f, precision, 0);
    ImGui::SliderInt("iterations", &iterations, 1, 32);

    ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
    if (ImGui::Button("Reset to default", ImVec2(m_buttonWidth, 0))) {
        amplitude = 0.0f; frequency = 0.0f; phase = 0.1f; iterations = 3;
    }
    ImGui::PopStyleColor(3);
    ImGui::End();

    m_simulationScene->getTerrain()->setParameters(amplitude, frequency, phase, iterations);
}

void SimulationInterface::renderAdditionalUI() {
	renderLightingPicker();
	renderSimulationSettings();
	renderTerrainSettings();
}

SimulationInterface::SimulationInterface(GLFWwindow* window, std::shared_ptr<Scene> scene) 
	: UserInterface(window, scene) {
    
	m_simulationScene = std::dynamic_pointer_cast<SimulationScene>(scene);
    if (!m_simulationScene) {
		throw std::runtime_error("'SimulationInterface' can only by initialized with 'SimulationScene' instance");
    }
}

