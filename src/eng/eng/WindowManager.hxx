#pragma once

import utl;
import glm;

#include <GLEW/glew.h>

#include <eng/Scene.hxx>
#include <eng/UserInterface.hxx>
#include <gl/DebugDrawer.hxx>

namespace lgl {
class WindowManager {
 private:
  GLFWwindow* m_mainWindow;

  const char* m_mainWindowName;
  std::uint32_t m_mainWindowWidth;
  std::uint32_t m_mainWindowHeight;
  std::uint32_t m_sceneWidth;
  std::uint32_t m_sceneHeight;

  std::shared_ptr<Scene> m_scene;
  std::shared_ptr<UserInterface> m_userInterface;

 private:
  void initMainWindow();
  void initOpenGl();
  void renderUserInterface();

 public:
  WindowManager(const char* mainWindowName, std::uint32_t mainWindowWidth,
                std::uint32_t mainWindowHeight, std::uint32_t sceneWidth,
                std::uint32_t sceneHeight);

  ~WindowManager();

  template <typename U, typename V>
  void defineLayout(const std::string& configName) {
    try {
      m_scene = std::make_shared<U>();
      m_scene->create();

      m_userInterface = std::make_shared<V>(m_mainWindow, m_scene);

      readConfig(configName);
    } catch (const std::exception& error) {
      throw error;
    }
  }

  void readConfig(const std::string& configName);
  void renderWindowLoop();
};
}  // namespace lgl