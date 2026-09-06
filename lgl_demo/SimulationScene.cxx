#include "SimulationScene.hxx"
#include "SimulationResourceRepository.hxx"

#include <collision/TerrainData.hxx>
#include <sceneobject/Box.hxx>

using namespace lgl;

void SimulationScene::initSimulationEvents() {
	using namespace keyin;

	m_simulationEventHandler.addEvent(KeyEvent<SimulationScene, float>(
		Key(GLFW_KEY_LEFT), &SimulationScene::advanceSimulation, -1.0f, false));

	m_simulationEventHandler.addEvent(KeyEvent<SimulationScene, float>(
		Key(GLFW_KEY_RIGHT), &SimulationScene::advanceSimulation, 1.0f, false));
}

void SimulationScene::castShadowsOnTerrain() {
	m_sun->directPerspectiveOf(*m_camera);

	int depthBufferWidth = m_resources->getBuffer("DEPTH")->getWidth();
	int depthBufferHeight = m_resources->getBuffer("DEPTH")->getHeight();
	glViewport(0, 0, depthBufferWidth, depthBufferHeight);

	m_terrain->getMesh()->setProgram(m_resources->getProgram("PATCH_DEPTH"));
	m_resources->getBuffer("DEPTH")->bind();
	for (const auto& s : m_sceneObjects) {
		if (s.get() == m_sun.get()) {
			continue;
		}
		s->draw(*m_camera);
	}
	m_resources->getBuffer("DEPTH")->unBind();
	m_terrain->getMesh()->setProgram(m_resources->getProgram("PATCH_PBR"));

	m_camera->resetProjection();
	m_camera->resetView();
}

void SimulationScene::rollbackToinitial() {
	/*utl::rdev rd;
	utl::rng gen(rd());
	utl::udist<float> momentumDist(-10.0f, 10.0f);
	utl::udist<float> angleDist(0.0f, 2.0f * 3.14159f);
	utl::udist<float> axisDist(-1.0f, 1.0f);*/

	for (int i = 2; i < m_sceneObjects.size(); ++i) {
		auto s = m_sceneObjects[i];
		s->getPhysicsSolver()->rollbackToInitial();
		s->updateTransformations();
	}

	/*for (int i = 2; i < m_sceneObjects.size(); ++i) {
		auto s = m_sceneObjects[i];
		glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
		s->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));
		s->getPhysicsSolver()->Body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
		s->getPhysicsSolver()->makeStateInitial();
	}*/
}

void SimulationScene::rollbackToPrevious() {
	for (int i = 2; i < m_sceneObjects.size(); ++i) {
		auto s = m_sceneObjects[i];
		s->getPhysicsSolver()->rollbackToPrevious();
		s->updateTransformations();
	}
}

void SimulationScene::advanceSimulation(float deltaTime) {
	for (const auto& sceneObject : m_sceneObjects) {
		sceneObject->stepPhysicsBy(deltaTime);
	}
}

void SimulationScene::setSimulationSpeed(float speed) {
	for (auto& event : m_simulationEventHandler.getArgEvents()) {
		auto& arg = event.getArgRef();
		arg = arg < 0.0f ? -speed : speed;
	}
}

void SimulationScene::create() {
	try {
		initSimulationEvents();
		setObjectMoveSpeed(0.05f);
		setSimulationSpeed(0.005f);

		m_resources = utl::makeUptr<SimulationResourceRepository>();

		m_sun = utl::makeSptr<Sun>(2.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_sun->setName("sun");
		m_sun->setProgram(m_resources->getProgram("TEX"));
		m_sun->setTextures({ m_resources->getTexture("SUN") });
		m_sun->translate(glm::vec3(15.0f, 50.0f, -15.0f));
		m_sceneObjects.push_back(m_sun);

		m_terrain = utl::makeSptr<Terrain>(1.0f, 4);
		m_terrain->setName("terrain");
		m_terrain->getTransformation()->setScaleMatrix(glm::vec3(400.0f, 50.0f, 400.0f));
		m_terrain->translate(glm::vec3(-200.0f, -20.0f, -200.0f));
		m_terrain->getMesh()->setProgram(m_resources->getProgram("PATCH_PBR"));
		m_terrain->getPhysicsSolver()->makeStateInitial();
		m_sceneObjects.push_back(m_terrain);

		for (int i = 0; i < 2; i++) {
			float s = 10.0f / (i + 1);
			auto woodBox = utl::makeSptr<Box>(false, glm::vec3(1.0f, 1.0f, 1.0f));
			woodBox->setName(utl::strFormat("wood_box_{}", i));
			woodBox->getMesh()->setProgram(m_resources->getProgram("PHONG"));
			woodBox->getMesh()->setMaterial(m_resources->getMaterial("WHITE"));
			woodBox->getMesh()->addTexture(m_resources->getTexture("WOOD"));
			woodBox->translate(glm::vec3(i, 10.0f + i * 10.0f, 0.0f));
			woodBox->scale(glm::vec3(s, s, s));
			woodBox->getPhysicsSolver()->makeStateInitial();
			m_sceneObjects.push_back(woodBox);
		}
		
		/*for (int i = 0; i < 2; i++) {
			float s = 10.0f / (i + 1);
			auto ironBox = utl::makeSptr<Box>(false, m_modelMeshes.at("IRONBOX"));
			ironBox->setName(utl::strFormat("iron_box_{}", i));
			ironBox->getMesh()->setProgram(m_programs.at("MPHONG"));
			ironBox->getMesh()->setMaterial(m_materials.at("WHITE"));
			ironBox->translate(glm::vec3(i, 10.0f + i * 10.0f, 0.0f));
			ironBox->scale(glm::vec3(s, s, s));
			ironBox->getPhysicsSolver()->makeStateInitial();
			m_sceneObjects.push_back(ironBox);
		}*/
	} 
	catch (const utl::except& error) {
		throw error;
	}
}

void SimulationScene::draw() const {
	CollisionHandler::debugDrawCollisions(m_sceneObjects, *m_camera);

	for (const auto& s : m_sceneObjects) {
		s->draw(*m_camera);
	}
	auto objectInFocus = m_sceneObjects[m_objectIndex];
	if (objectInFocus.get() != m_terrain.get()) {
		objectInFocus->getMesh()->drawMeshFrame(*m_camera);
	}
}

void SimulationScene::update(GLFWwindow* window) {
	try {
		if (m_moveCamera) {
			m_cameraEventHandler.handleEvents(window, m_camera.get());
			m_camera->resetView();
			m_camera->resetProjection();
			if (Mouse::s_isCaptured) {
				m_camera->mouseUpdate();
			}
		}
		else {
			m_objectEventHandler.handleEvents(window, m_sceneObjects[m_objectIndex].get());
		}

		m_resources->handleShaderChanges();

		m_terrain->updateTerrainParameters();

		m_simulationEventHandler.handleEvents(window, this);
		CollisionHandler::handleCollisions(m_sceneObjects);

		castShadowsOnTerrain();
		m_sun->lightUpScene(m_sceneObjects, *m_resources->getBuffer("DEPTH"));
	}
	catch (const utl::except& error) {
		Logger::log(Logger::LGL_ERROR, "Simulation scene update exception: {}\n", error.what());
	}
	catch (...) {
		Logger::log(Logger::LGL_ERROR, "Simulation scene update exception\n");
	}
}
