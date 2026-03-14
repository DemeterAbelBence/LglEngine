#pragma once

import lglmodule;
import glmmodule;

#include <eng/Scene.hxx>
#include <sceneobject/Sun.hxx>
#include <sceneobject/Terrain.hxx>
#include <sceneobject/Box.hxx>
#include <collision/TerrainData.hxx>

class SimulationScene : public lgl::Scene {
private:
	lgl::utl::sptr<lgl::Sun> m_sun;
	lgl::utl::sptr<lgl::Terrain> m_terrain;

	lgl::keyin::KeyEventHandler<SimulationScene, float> m_simulationEventHandler;

private:
	void initSimulationEvents();
	void initResources();
	void castShadowsOnTerrain();

public:
	SimulationScene() : lgl::Scene() {}

	const lgl::utl::sptr<lgl::Terrain>& getTerrain() const { return m_terrain; }
	const lgl::utl::sptr<lgl::Sun>& getSun() const { return m_sun; }
	const auto& getSimulationEventHandler() { return m_simulationEventHandler; }

	void rollbackSimulation();
	void advanceSimulation(float deltaTime);
	void setSimulationSpeed(float speed);

	void create() override;
	void draw() const override;
	void update(GLFWwindow* window) override;
};