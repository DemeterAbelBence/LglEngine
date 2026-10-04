#pragma once

import utl;
import glm;

#include <eng/Scene.hxx>
#include <sceneobject/Sun.hxx>
#include <sceneobject/Terrain.hxx>

class SimulationScene final : public lgl::Scene {
 private:
  std::shared_ptr<lgl::Sun> m_sun;                            ///< lorem ipsum
  std::shared_ptr<lgl::Terrain> m_terrain;                    ///< lorem ipsum
  std::unique_ptr<lgl::CollisionHandler> m_collisionHandler;  ///< lorem ipsum

  lgl::keyin::KeyEventHandler<SimulationScene, float> m_simulationEventHandler;  ///< lorem ipsum

 public:
  inline static bool s_randomizeRollback = false;        ///< lorem ipsum
  inline static bool s_lockSimulation = false;           ///< lorem ipsum
  inline static bool s_saveObjectStates = false;         ///< lorem ipsum
  inline static bool s_continousCollision = false;       ///< lorem ipsum
  inline static std::uint32_t s_simulationDelay = 0ui32; ///< lorem ipsum

 private:
  void initSimulationEvents();
  void castShadowsOnTerrain();

 public:
  SimulationScene();
  ~SimulationScene();

  const std::shared_ptr<lgl::Terrain>& getTerrain() const { return m_terrain; }
  const std::shared_ptr<lgl::Sun>& getSun() const { return m_sun; }
  const std::unique_ptr<lgl::CollisionHandler>& getCollisionHandler() const {
    return m_collisionHandler;
  }
  const auto& getSimulationEventHandler() { return m_simulationEventHandler; }
  void handleCollisions() { m_collisionHandler->handleCollisions(m_sceneObjects); }

  void rollbackToInitial();
  void rollbackToPrevious();
  void advanceSimulation(float deltaTime);
  void setSimulationSpeed(float speed);

  void readConfig(const utl::LglConf& lglConf) override;
  void draw() const override;
  void update(GLFWwindow* window) override;
};