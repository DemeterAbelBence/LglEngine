#include "SimulationScene.hpp"

using namespace lgl;

void SimulationScene::initSimulationEvents() {
	using namespace keyin;

	m_simulationEventHandler.addEvent(KeyEvent<SimulationScene, float>(
		Key(GLFW_KEY_LEFT), &SimulationScene::advanceSimulation, -1.0f, false));

	m_simulationEventHandler.addEvent(KeyEvent<SimulationScene, float>(
		Key(GLFW_KEY_RIGHT), &SimulationScene::advanceSimulation, 1.0f, false));
}

void SimulationScene::initResources() {
	try {
		// PATCH PHONG PROGRAM
		auto patchPhong = std::make_shared<GpuProgram>("PATCH_PHONG");
		patchPhong->addShader(GL_FRAGMENT_SHADER, "shaders/patch/patch-frag-phong.glsl");
		patchPhong->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchPhong->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchPhong->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchPhong->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchPhong->createProgram();
		m_programs[patchPhong->getProgramName()] = patchPhong;

		// PATCH DEPTH PROGRAM
		auto patchDepth = std::make_shared<GpuProgram>("PATCH_DEPTH");
		patchDepth->addShader(GL_FRAGMENT_SHADER, "shaders/basic/depth-frag.glsl");
		patchDepth->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchDepth->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchDepth->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchDepth->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchDepth->createProgram();
		m_programs[patchDepth->getProgramName()] = patchDepth;

		// PATCH PBR PROGRAM
		auto patchPBR = std::make_shared<GpuProgram>("PATCH_PBR");
		patchPBR->addShader(GL_FRAGMENT_SHADER, "shaders/patch/patch-frag-pbr.glsl");
		patchPBR->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchPBR->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchPBR->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchPBR->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchPBR->createProgram();
		m_programs[patchPBR->getProgramName()] = patchPBR;

		// PHONG PROGRAM
		auto phong = std::make_shared<GpuProgram>("PHONG");
		phong->addShader(GL_VERTEX_SHADER, "shaders/phong-vert.glsl");
		phong->addShader(GL_FRAGMENT_SHADER, "shaders/phong-frag.glsl");
		phong->createProgram();
		m_programs[phong->getProgramName()] = phong;

		// TEX PROGRAM
		auto texProgram= std::make_shared<GpuProgram>("TEX");
		texProgram->addShader(GL_FRAGMENT_SHADER, "shaders/basic/tex-frag.glsl");
		texProgram->addShader(GL_VERTEX_SHADER, "shaders/basic/tex-vert.glsl");
		texProgram->createProgram();
		m_programs[texProgram->getProgramName()] = texProgram;

		// SUN TEXTURE
		m_textures["SUN"] = std::make_shared<Texture>(Texture{
			tex::TextureFromFile("assets/images/sun.jpg"),
			"sun_texture", ""
		});

		// WHITE MATERIAL
		m_materials["WHITE"] = std::make_shared<Material>(Material{
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			10.0f
		});

		// CUBE MODEL MESH
		auto cubeModel = std::make_shared<ModelMesh>("assets/cube/Cube_obj.obj", true, true);
		cubeModel->scaleModelBy(glm::vec3(0.007f, 0.007f, 0.007f));
		m_modelMeshes["IRONBOX"] = cubeModel;

		// DEPTH BUFFER
		m_buffers["DEPTH"] = std::make_shared<FrameBuffer>("DEPTH_MAP", 800, 800,
			GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT);
	}
	catch (const std::exception& error) {
		throw error;
	}
}

void SimulationScene::castShadowsOnTerrain() {
	m_sun->directPerspectiveOf(*m_camera);

	m_terrain->getMesh()->setProgram(m_programs["PATCH_DEPTH"]);
	m_buffers["DEPTH"]->bind();
	for (const auto& s : m_sceneObjects) {
		if (s == m_sun) {
			continue;
		}
		s->draw(*m_camera);
	}
	m_buffers["DEPTH"]->unBind();
	m_terrain->getMesh()->setProgram(m_programs["PATCH_PBR"]);

	m_camera->resetProjection();
	m_camera->resetView();
}

void SimulationScene::rollbackSimulation() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> momentumDist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
	std::uniform_real_distribution<float> axisDist(-1.0f, 1.0f);

	for (int i = 2; i < m_sceneObjects.size(); ++i) {
		auto s = m_sceneObjects[i];
		s->resetBodyState();
	}

	//for (int i = 2; i < m_sceneObjects.size(); ++i) {
	//	auto s = m_sceneObjects[i];
	//	glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
	//	s->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));
	//	s->getPhysicsSolver()->Body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
	//	s->getPhysicsSolver()->makeStateInitial();
	//}
}

void SimulationScene::advanceSimulation(float deltaTime) {
	for (const auto& sceneObject : m_sceneObjects) {
		sceneObject->stepPhysicsBy(deltaTime);
	}
}

void SimulationScene::setSimulationSpeed(float speed) {
	for (auto& event : m_simulationEventHandler.getArgEvents()) {
		event.getArgRef() = speed;
	}
}

void SimulationScene::create() {
	try {
		initResources();
		initSimulationEvents();
		setObjectMoveSpeed(0.05f);
		setSimulationSpeed(0.005f);

		m_sun = std::make_shared<Sun>(2.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_sun->m_name = std::string("sun");
		m_sun->setProgram(m_programs.at("TEX"));
		m_sun->setTextures({ m_textures.at("SUN") });
		m_sun->translate(glm::vec3(5.0f, 60.0f, 5.0f));
		m_sceneObjects.push_back(m_sun);

		m_terrain = std::make_shared<Terrain>(1.0f, 4);
		m_terrain->m_name = std::string("terrain");
		m_terrain->getTransformation()->setScaleMatrix(glm::vec3(400.0f, 50.0f, 400.0f));
		m_terrain->translate(glm::vec3(-200.0f, -20.0f, -200.0f));
		m_terrain->getMesh()->setProgram(m_programs.at("PATCH_PBR"));
		m_terrain->getPhysicsSolver()->makeStateInitial();
		m_sceneObjects.push_back(m_terrain);

		for (int i = 0; i < 3; i++) {
			float s = 10.0f / (i + 1);
			auto ironBox = std::make_shared<Box>(false, m_modelMeshes.at("IRONBOX"));
			ironBox->m_name = std::string("ironbox") + std::to_string(i);
			ironBox->getMesh()->setProgram(m_programs.at("PHONG"));
			ironBox->getMesh()->setMaterial(m_materials.at("WHITE"));
			ironBox->translate(glm::vec3(i, 10.0f + i * 10.0f, 0.0f));
			ironBox->scale(glm::vec3(s, s, s));
			ironBox->getPhysicsSolver()->makeStateInitial();
			m_sceneObjects.push_back(ironBox);
		}
	} 
	catch (const std::exception& error) {
		throw error;
	}
}

void SimulationScene::draw() const {
	for (const auto& s : m_sceneObjects) {
		s->draw(*m_camera);
	}
	auto objectInFocus = m_sceneObjects[m_objectIndex];
	if (objectInFocus != m_terrain) {
		auto transformation = objectInFocus->getTransformation();
		objectInFocus->getMesh()->setTransformation(transformation);
		objectInFocus->getMesh()->drawMeshFrame(*m_camera);
	}
}

void SimulationScene::update(GLFWwindow* window) {
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

	for (const auto& program : m_programs) {
		program.second->handleShaderSourceChange();
	}

	m_terrain->updateTerrainParameters();
	
	castShadowsOnTerrain();
	m_sun->lightUpScene(m_sceneObjects, *m_buffers["DEPTH"]);

	CollisionHandler::handleCollisions(m_sceneObjects, *m_camera);

	m_simulationEventHandler.handleEvents(window, this);
}
