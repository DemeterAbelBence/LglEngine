#include <eng/WindowManager.hxx>

#include "SimulationInterface.hxx"
#include "SimulationScene.hxx"

int main(int argc, char** argv) {
  try {
    lgl::WindowManager windowManager("Simulation App", 1200, 800, 800, 800);
    windowManager.defineLayout<SimulationScene, SimulationInterface>(
        argc > 1 ? argv[1] : "");
    windowManager.renderWindowLoop();

  } catch (const std::exception& error) {
    utl::log(utl::GENERAL_LOGS, utl::LGL_ERROR, "Exception: {}\n",
             error.what());
  }

  return 0;
}