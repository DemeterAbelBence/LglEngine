#include <iostream>

#include <eng/WindowManager.hpp>

#include "SimulationScene.hpp"
#include "SimulationInterface.hpp"
#include "SponzaScene.hpp"
#include "LightingInterface.hpp"

int main(void) {
	try {
		lgl::WindowManager windowManager("Simulation App", 1200, 800, 800, 800);
		windowManager.defineLayout<SimulationScene, SimulationInterface>();
		//windowManager.defineLayout<SponzaScene, LightingInterface>();
		windowManager.renderWindowLoop();
	}
	catch (const std::exception& error) {
		std::cout << error.what() << std::endl;
	}

    return 0;
}