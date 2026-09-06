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
	static float depthBias = 0.001f;
    static utl::cstr precision = "%.3f";

    ImGui::Begin("Simulation settings");

	ImGui::Columns(2, "toggle_columns", false);

	bool ed = CollisionHandler::enableDebug;
	utl::cstr debugText = ed ? "Disable Debug" : "Enable Debug";
	if (ImGui::Button(debugText)) { CollisionHandler::enableDebug = !ed; }

	ImGui::NextColumn();

	bool ebis = CollisionHandler::enableBisection;
	utl::cstr bisectionText = ebis ? "Disable bisection" : "Enable bisection";
	if (ImGui::Button(bisectionText)) { CollisionHandler::enableBisection = !ebis; }

	ImGui::NextColumn();

	bool eimp = CollisionHandler::enableImpulses;
	utl::cstr impulsesText = eimp ? "Disable impulses" : "Enable impulses";
	if (ImGui::Button(impulsesText)) { CollisionHandler::enableImpulses = !eimp; }

	ImGui::NextColumn();

	bool erf = CollisionHandler::enableRestingForces;
	utl::cstr restingForcesText = erf ? "Disable resting forces" : "Enable resting forces";
	if (ImGui::Button(restingForcesText)) { CollisionHandler::enableRestingForces = !erf; }

	ImGui::NextColumn();

	bool epap = CollisionHandler::enablePushingApart;
	utl::cstr pushingApartText = epap ? "Disable pushing apart" : "Enable pushing apart";
	if (ImGui::Button(pushingApartText)) { CollisionHandler::enablePushingApart = !epap; }

    bool dn = CollisionHandler::drawNormals;
    utl::cstr drawNormalsText = dn ? "Draw normals" : "Hide normals";
    if (ImGui::Button(drawNormalsText)) { CollisionHandler::drawNormals = !dn; }

	ImGui::NextColumn();

	if (ImGui::Button("Push apart once")) { CollisionHandler::pushApartOnce = true; }
    if (ImGui::Button("Log states once")) { CollisionHandler::logStatesOnce = true; }

	ImGui::Columns(1);

    ImGui::Separator();

    bool efl = Logger::logToFile;
    utl::cstr fileLogText = efl ? "Disable file logging" : "Enable file logging";
    if (ImGui::Button(fileLogText)) {
        Logger::logToFile = !efl;
        lgl::Logger::setLogMode(lgl::Logger::GENERAL_LOGS);
    }

    bool ecl = CollisionHandler::enableContactLog;
    utl::cstr contactLogText = ecl ? "Disable contact log" : "Enable contact log";
    if (ImGui::Button(contactLogText)) {
        CollisionHandler::enableContactLog = !ecl;
    }

    bool ebl = CollisionHandler::enableBisectionLog;
    utl::cstr bisectionLogText = ebl ? "Disable bisection log" : "Enable bisection log";
    if (ImGui::Button(bisectionLogText)) {
        CollisionHandler::enableBisectionLog = !ebl;
    }

    ImGui::Separator();

    if (ImGui::Button("Simulate forward")) { m_simulationScene->advanceSimulation(Time::s_fixedDeltaTime); }
    if (ImGui::Button("Simulate backward")) { m_simulationScene->advanceSimulation(-Time::s_fixedDeltaTime); }
    if (ImGui::Button("Rollback to initial")) { m_simulationScene->rollbackToinitial(); }
    if (ImGui::Button("Rollback to previous")) { m_simulationScene->rollbackToPrevious(); }

	/*SceneObject* currentObject = m_simulationScene->getCurrentSceneObject().get();
    if(CollisionHandler::isFrozen(currentObject)) {
        if (ImGui::Button("Unfreeze current")) {
            CollisionHandler::unfreezeObject(currentObject);
        }
    }
    else {
        if (ImGui::Button("Freeze current")) {
            CollisionHandler::freezeObject(currentObject);
        }
	}*/

    ImGui::Separator();

    if (ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.001f, 0.05f, precision, 0)) {
		Time::s_fixedDeltaTime = simulationSpeed;
        m_simulationScene->setSimulationSpeed(simulationSpeed);
    }

    ImGui::SliderFloat("Elasticity", &CollisionHandler::elasticity, 0.0f, 1.0f, precision, 0);
    ImGui::SliderFloat("Bisection Bias", &CollisionHandler::bisectionBias, 0.0f, 0.01f, "%.5f", 0);
    ImGui::SliderFloat("Contact Bias", &CollisionHandler::contactBias, 0.0f, 0.01f, "%.5f", 0);
	ImGui::SliderFloat("PSD Tolerance", &CollisionHandler::PSDTolerance, 0.0f, 0.001f, "%.7f", 0);
    ImGui::SliderInt("logFrequency", reinterpret_cast<int*>(&Logger::logFrequency), 1, 32);

    ImGui::Separator();

    static char sceneSaveName[128] = "";
    ImGui::InputText("Scene Save Name", sceneSaveName, IM_ARRAYSIZE(sceneSaveName));
    
    if (ImGui::Button("Save Scene")) {
        utl::str savePath = utl::strFormat("{}/{}", ribo::baseSerializationPath, sceneSaveName);
        m_simulationScene->saveObjectStates(savePath);
    }

    if (ImGui::Button("Load Scene")) {
        utl::str savePath = utl::strFormat("{}/{}", ribo::baseSerializationPath, sceneSaveName);
		m_simulationScene->loadObjectStates(savePath);
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

