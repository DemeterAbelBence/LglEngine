#include <eng/CollisionHandlerV1.hxx>

#include "SimulationInterface.hxx"

using namespace lgl;

void SimulationInterface::renderLightingPicker() {
  static float lightPowerDens[3] = {1.0f, 1.0f, 1.0f};

  ImGui::Begin("Light picker");
  ImGui::SliderFloat3("lightPowerDens", lightPowerDens, 0.0f, 1000.0f, "%.2f", 0);
  ImGui::End();

  m_simulationScene->getSun()->getLight()->powerDensity =
      glm::vec3(lightPowerDens[0], lightPowerDens[1], lightPowerDens[2]);
}

void SimulationInterface::renderSimulationSettings() {
  static float simulationSpeed = 0.005f;
  static const char* precision = "%.3f";

  ImGui::Begin("Simulation settings");

  // onToggle is a plain function pointer
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

  toggleButton(CollisionHandlerV1::s_enableDebug, "Disable Debug", "Enable Debug");

  ImGui::NextColumn();

  toggleButton(CollisionHandlerV1::s_enableBisection, "Disable bisection", "Enable bisection");

  ImGui::NextColumn();

  toggleButton(CollisionHandlerV1::s_enableImpulses, "Disable impulses", "Enable impulses");

  ImGui::NextColumn();

  toggleButton(CollisionHandlerV1::s_enableRestingForces, "Disable resting forces", "Enable resting forces");

  ImGui::NextColumn();

  toggleButton(CollisionHandlerV1::s_enableDepenetration, "Disable depenetration", "Enable depenetration");

  toggleButton(CollisionHandlerV1::s_drawVectors, "Hide vectors", "Draw vectors");

  toggleButton(m_simulationScene->s_randomizeRollback, "Disable random rollback", "Enable random rollback");

  ImGui::NextColumn();

  ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
  if (ImGui::Button("Push apart once", ImVec2(m_buttonWidth, 0))) {
    CollisionHandlerV1::s_pushApartOnce = true;
    m_simulationScene->handleCollisions();
  }
  if (ImGui::Button("Log states to console", ImVec2(m_buttonWidth, 0))) {
    m_simulationScene->getCollisionHandler()->logPhysicsStateOfObjects(true, m_simulationScene->getSceneObjects());
  }
  ImGui::PopStyleColor(3);

  toggleButton(m_simulationScene->s_lockSimulation, "Unlock simulation", "Lock simulation");

  ImGui::Columns(1);

  ImGui::Separator();

  // Physics log toggle
  toggleButton(CollisionHandlerV1::s_enablePhysicsLog, "Disable physics log", "Enable physics log", nullptr);

  // Contact log toggle
  toggleButton(CollisionHandlerV1::s_enableContactLog, "Disable contact log", "Enable contact log", nullptr);

  // Bisection log toggle
  toggleButton(CollisionHandlerV1::s_enableBisectionLog, "Disable bisection log", "Enable bisection log", nullptr);

  ImGui::NextColumn();

  ImGui::Separator();

  ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
  if (ImGui::Button("Simulate forward", ImVec2(m_buttonWidth, 0))) {
    m_simulationScene->advanceSimulation(Time::s_fixedDeltaTime);
  }
  if (ImGui::Button("Simulate backward", ImVec2(m_buttonWidth, 0))) {
    m_simulationScene->advanceSimulation(-Time::s_fixedDeltaTime);
  }
  if (ImGui::Button("Rollback to initial", ImVec2(m_buttonWidth, 0))) {
    m_simulationScene->rollbackToInitial();
  }
  if (ImGui::Button("Rollback to previous", ImVec2(m_buttonWidth, 0))) {
    m_simulationScene->rollbackToPrevious();
  }

  ImGui::PopStyleColor(3);

  ImGui::Separator();

  if (ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.001f, 0.05f, precision, 0)) {
    m_simulationScene->setSimulationSpeed(simulationSpeed);
  }

  ImGui::SliderInt("Simulation Delay", reinterpret_cast<int*>(&SimulationScene::s_simulationDelay), 0, 500);

  ImGui::SliderFloat("Elasticity", &CollisionHandlerV1::s_elasticity, 0.0f, 1.0f, precision, 0);
  ImGui::SliderFloat("Bisection Bias", &CollisionHandlerV1::s_bisectionBias, 0.0f, 0.01f, "%.5f", 0);
  ImGui::SliderFloat("Contact Bias", &CollisionHandlerV1::s_contactBias, 0.0f, 0.01f, "%.5f", 0);
  ImGui::SliderFloat("PSD Tolerance", &CollisionHandlerV1::s_PSDTolerance, 0.0f, 0.001f, "%.7f", 0);

  ImGui::Separator();

  static char sceneSaveName[128] = "";
  ImGui::InputText("Scene Save Name", sceneSaveName, IM_ARRAYSIZE(sceneSaveName));

  ImGui::PushStyleColor(ImGuiCol_Button, m_blueBtn);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_blueBtnHov);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_blueBtnAct);
  if (ImGui::Button("Save Scene", ImVec2(m_buttonWidth, 0))) {
    std::string savePath = std::format("{}/saves/{}", utl::lglResourceFolder, sceneSaveName);
    m_simulationScene->saveObjectStates(savePath);
  }

  if (ImGui::Button("Load Scene", ImVec2(m_buttonWidth, 0))) {
    std::string savePath = std::format("{}/saves/{}", utl::lglResourceFolder, sceneSaveName);
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
    amplitude = 0.0f;
    frequency = 0.0f;
    phase = 0.1f;
    iterations = 3;
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
    throw std::runtime_error(
        "'SimulationInterface' can only by initialized with "
        "'SimulationScene' "
        "instance");
  }
}
