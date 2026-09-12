#pragma once

import utl;
import glm;

#include <eng/Scene.hxx>
#include <sceneobject/Sun.hxx>
#include <sceneobject/Terrain.hxx>

class SimulationScene final : public lgl::Scene {
private:
	std::shared_ptr<lgl::Sun> m_sun;
	std::shared_ptr<lgl::Terrain> m_terrain;

	lgl::keyin::KeyEventHandler<SimulationScene, float> m_simulationEventHandler;

public:
	inline static bool s_randomizeRollback = false;
	inline static bool s_lockSimulation = true;


private:
	void initSimulationEvents();
	void castShadowsOnTerrain();

public:
	SimulationScene() : lgl::Scene() {}

	const std::shared_ptr<lgl::Terrain>& getTerrain() const { return m_terrain; }
	const std::shared_ptr<lgl::Sun>& getSun() const { return m_sun; }
	const auto& getSimulationEventHandler() { return m_simulationEventHandler; }
	void handleCollisions() { lgl::CollisionHandler::handleCollisions(m_sceneObjects); }

	void loadFromFile(const std::string& path);
	void rollbackToinitial();
	void rollbackToPrevious();
	void advanceSimulation(float deltaTime);
	void setSimulationSpeed(float speed);

	void create() override;
	void draw() const override;
	void update(GLFWwindow* window) override;
};