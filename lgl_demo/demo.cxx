#include <eng/WindowManager.hxx>

#include "SimulationScene.hxx"
#include "SimulationInterface.hxx"

int main(void) {
	try {
		utl::log(utl::LGL_INFO, "Starting application...\n");

		lgl::WindowManager windowManager("Simulation App", 1200, 800, 800, 800);
		windowManager.defineLayout<SimulationScene, SimulationInterface>();
		windowManager.renderWindowLoop();
	}
	catch (const std::exception& error) {
		utl::log(utl::LGL_ERROR, "Exception: {}\n", error.what());
	}

	utl::log(utl::LGL_INFO, "Exiting application...\n");

    return 0;
}