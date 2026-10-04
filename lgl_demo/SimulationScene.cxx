#include "SimulationScene.hxx"

#include <collision/TerrainData.hxx>
#include <sceneobject/Box.hxx>
#include <eng/CollisionHandlerV1.hxx>

#include "SimulationResourceRepository.hxx"

using namespace lgl;

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::initSimulationEvents() {
  using namespace keyin;

  m_simulationEventHandler.addEvent(
      KeyEvent<SimulationScene, float>(Key(GLFW_KEY_LEFT), &SimulationScene::advanceSimulation, -1.0f, false));

  m_simulationEventHandler.addEvent(
      KeyEvent<SimulationScene, float>(Key(GLFW_KEY_RIGHT), &SimulationScene::advanceSimulation, 1.0f, false));
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
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

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
SimulationScene::SimulationScene() : lgl::Scene() {
  try {
    initSimulationEvents();
    setObjectMoveSpeed(0.05f);
    setSimulationSpeed(0.005f);

    m_resources = std::make_unique<SimulationResourceRepository>();

    m_collisionHandler = std::make_unique<lgl::CollisionHandlerV1>();

    m_sun = std::make_shared<Sun>(2.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    m_sun->setName("sun");
    m_sun->setProgram(m_resources->getProgram("TEX"));
    m_sun->setTextures({m_resources->getTexture("SUN")});
    m_sun->translate(glm::vec3(15.0f, 50.0f, -15.0f));
    m_sceneObjects.push_back(m_sun);

    m_terrain = std::make_shared<Terrain>(1.0f, 4);
    m_terrain->setName("terrain");
    m_terrain->getTransformation()->setScaleMatrix(glm::vec3(400.0f, 50.0f, 400.0f));
    m_terrain->translate(glm::vec3(-200.0f, -20.0f, -200.0f));
    m_terrain->getMesh()->setProgram(m_resources->getProgram("PATCH_PBR"));
    m_terrain->getPhysicsSolver()->makeStateInitial();
    m_sceneObjects.push_back(m_terrain);

    for (int i = 0; i < 1; i++) {
      float s = 10.0f / (i + 1);
      auto woodBox = std::make_shared<Box>(false);
      woodBox->setName(std::format("wood_box_{}", i));
      woodBox->getMesh()->setProgram(m_resources->getProgram("PHONG"));
      woodBox->getMesh()->setMaterial(m_resources->getMaterial("WHITE"));
      woodBox->getMesh()->addTexture(m_resources->getTexture("WOOD"));
      woodBox->translate(glm::vec3(i, 10.0f + i * 10.0f, 0.0f));
      woodBox->scale(glm::vec3(s, s, s));
      woodBox->getPhysicsSolver()->makeStateInitial();
      m_sceneObjects.push_back(woodBox);
    }
  } catch (const std::exception& error) {
    throw error;
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
SimulationScene::~SimulationScene() {
  if (s_saveObjectStates) {
    saveObjectStates(std::format("{}/saves/save{}", utl::lglResourceFolder, utl::lglStartTime));
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::rollbackToInitial() {
  for (int i = 2; i < m_sceneObjects.size(); ++i) {
    auto s = m_sceneObjects[i];
    s->getPhysicsSolver()->rollbackToInitial();
    s->updateTransformations();
  }

  if (s_randomizeRollback) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> momentumDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
    std::uniform_real_distribution<float> axisDist(-1.0f, 1.0f);

    for (int i = 2; i < m_sceneObjects.size(); ++i) {
      auto s = m_sceneObjects[i];
      glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
      s->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));
      s->getPhysicsSolver()->m_body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
      s->getPhysicsSolver()->makeStateInitial();
    }
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::rollbackToPrevious() {
  for (int i = 2; i < m_sceneObjects.size(); ++i) {
    auto s = m_sceneObjects[i];
    s->getPhysicsSolver()->rollbackToPrevious();
    s->updateTransformations();
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::advanceSimulation(float deltaTime) {
  for (const auto& sceneObject : m_sceneObjects) {
    sceneObject->stepPhysicsBy(deltaTime);
  }
  m_collisionHandler->handleCollisions(m_sceneObjects);
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::setSimulationSpeed(float speed) {
  Time::s_fixedDeltaTime = speed;
  for (auto& event : m_simulationEventHandler.getArgEvents()) {
    auto& arg = event.getArgRef();
    arg = arg < 0.0f ? -speed : speed;
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::readConfig(const utl::LglConf& lglConf) {
  try {
    m_collisionHandler->readConfig(lglConf);

    s_lockSimulation = lglConf.getBooleanConfig("lockSimulation");
    s_saveObjectStates = lglConf.getBooleanConfig("saveObjectStates");
    s_randomizeRollback = lglConf.getBooleanConfig("enableRandomRollback");
    s_simulationDelay = lglConf.getIntConfig("simulationDelay");

    setSimulationSpeed(lglConf.getFloatConfig("simulationSpeed"));

    loadObjectStates(std::format("{}/saves/{}", utl::lglResourceFolder,
                                 lglConf.getStringConfig("sceneSave")));
    for (const auto& s : m_sceneObjects) {
      s->getPhysicsSolver()->makeStateInitial();
      s->getPhysicsSolver()->initForces();
      s->updateTransformations();
    }
  } catch (const std::exception& error) {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "Simulation scene configuration reading error:\n");
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "{}\n", error.what());
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::draw() const {
  m_collisionHandler->debugContacts(*m_camera, true);
  //m_collisionHandler->drawCollidersOf(m_sceneObjects, *m_camera);

  for (const auto& s : m_sceneObjects) {
    s->draw(*m_camera);
  }

  auto objectInFocus = m_sceneObjects[m_objectIndex];
  if (objectInFocus.get() != m_terrain.get()) {
    objectInFocus->getMesh()->drawMeshFrame(*m_camera);
  }
}

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
 * labore et dolore magna aliqua.
 */
void SimulationScene::update(GLFWwindow* window) {
  try {
    if (m_moveCamera) {
      m_cameraEventHandler.handleEvents(window, m_camera.get());
      m_camera->resetView();
      m_camera->resetProjection();
      if (Mouse::s_isCaptured) {
        m_camera->mouseUpdate();
      }
    } else {
      m_objectEventHandler.handleEvents(window, m_sceneObjects[m_objectIndex].get());
    }

    m_resources->handleShaderChanges();

    m_terrain->updateTerrainParameters();

    castShadowsOnTerrain();
    m_sun->lightUpScene(m_sceneObjects, *m_resources->getBuffer("DEPTH"));

    advanceSimulation(Time::s_fixedDeltaTime);

    if (s_lockSimulation) {
      advanceSimulation(Time::s_fixedDeltaTime);
    } else {
      m_simulationEventHandler.handleEvents(window, this);
    }

    if (s_simulationDelay > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(s_simulationDelay));
    }
  } catch (const std::exception& error) {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "Simulation scene update exception: {}\n", error.what());
  } catch (...) {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "Simulation scene update exception\n");
  }
}
