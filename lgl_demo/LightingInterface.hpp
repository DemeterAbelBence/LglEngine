#pragma once

#include <gl/BaseDefs.hpp>

#include <eng/UserInterface.hpp>
#include <eng/Scene.hpp>

using namespace lgl;

class LightingInterface : public UserInterface {
private:
	SPTR<Sun> m_sun;

private:
	void renderLightingPicker();

protected:
	void renderAdditionalUI() override;

public:
	LightingInterface(GLFWwindow* window, SPTR<Scene> scene);
};