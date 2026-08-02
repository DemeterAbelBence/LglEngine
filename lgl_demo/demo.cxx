#include <eng/WindowManager.hxx>

#include "SimulationScene.hxx"
#include "SimulationInterface.hxx"
#include "SponzaScene.hxx"
#include "LightingInterface.hxx"

import qpmodule;

int main(void) {
	try {
		lgl::Logger::log(lgl::Logger::LGL_INFO, "Starting application...\n");

		lgl::WindowManager windowManager("Simulation App", 1200, 800, 800, 800);
		windowManager.defineLayout<SimulationScene, SimulationInterface>();
		//windowManager.defineLayout<SponzaScene, LightingInterface>();
		windowManager.renderWindowLoop();
	}
	catch (const lgl::utl::except& error) {
		lgl::Logger::log(lgl::Logger::LGL_ERROR, "Exception: {}\n", error.what());
	}

	lgl::Logger::log(lgl::Logger::LGL_INFO, "Maximum resting force applied: {}\n", lgl::CollisionHandler::maxRestingForceApplied);

	lgl::Logger::log(lgl::Logger::LGL_EMPTY, "Exiting application...\n");

	//qp::solveDummyQP();

    return 0;
}