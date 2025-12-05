#pragma once

#include <eng/Scene.hpp>

#include <sceneobject/Sun.hpp>
#include <sceneobject/Terrain.hpp>
#include <sceneobject/Box.hpp>

#include <collision/TerrainData.hpp>

using namespace lgl;

class SimulationScene : public Scene {
private:
	SPTR<Sun> m_sun;
	SPTR<Terrain> m_terrain;

	keyin::KeyEventHandler<SimulationScene, float> m_simulationEventHandler;

private:
	void initSimulationEvents();
	void initResources();
	void castShadowsOnTerrain();

public:
	SimulationScene() : Scene() {}

	const SPTR<Terrain>& getTerrain() const { return m_terrain; }
	const SPTR<Sun>& getSun() const { return m_sun; }
	const auto& getSimulationEventHandler() { return m_simulationEventHandler; }

	void rollbackSimulation();
	void advanceSimulation(float deltaTime);
	void setSimulationSpeed(float speed);

	void create() override;
	void draw() const override;
	void update(GLFWwindow* window) override;
};