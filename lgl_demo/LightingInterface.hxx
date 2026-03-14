#pragma once

import lglmodule;
import glmmodule;

#include <eng/UserInterface.hxx>
#include <eng/Scene.hxx>

class LightingInterface : public lgl::UserInterface {
private:
	lgl::utl::sptr<lgl::Sun> m_sun;

private:
	void renderLightingPicker();

protected:
	void renderAdditionalUI() override;

public:
	LightingInterface(GLFWwindow* window, lgl::utl::sptr<lgl::Scene> scene);
};