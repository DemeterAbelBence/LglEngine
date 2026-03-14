#pragma once

import lglmodule;
import glmmodule;

#include <eng/Scene.hxx>
#include <visual/mesh/ModelMesh.hxx>
#include <sceneobject/Sun.hxx>

class SponzaScene : public lgl::Scene {
private:
	lgl::utl::sptr<lgl::Sun> m_sun;

private:
	void initResources();

public:
	SponzaScene() : lgl::Scene() {}

	const lgl::utl::sptr<lgl::Sun>& getSun() const { return m_sun; }

	void create() override;
	void draw() const override;
	void update(GLFWwindow* window) override;
};