#pragma once

#include <gl/BaseDefs.hpp>

#include <eng/Scene.hpp>

#include <visual/mesh/ModelMesh.hpp>

#include <sceneobject/Sun.hpp>

using namespace lgl;

class SponzaScene : public Scene {
private:
	SPTR<Sun> m_sun;

private:
	void initResources();

public:
	SponzaScene() : Scene() {}

	const SPTR<Sun>& getSun() const { return m_sun; }

	void create() override;
	void draw() const override;
	void update(GLFWwindow* window) override;
};