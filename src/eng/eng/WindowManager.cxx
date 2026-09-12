#include "WindowManager.hxx"

#include <cstring>

namespace lgl {
void WindowManager::initMainWindow() {
  GLFWwindow* window;
  try {
    if (!glfwInit()) {
      throw std::exception("Glfw initialization error!\n");
    }

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    window = glfwCreateWindow(m_mainWindowWidth, m_mainWindowHeight,
                              m_mainWindowName, NULL, NULL);
    if (!window) {
      glfwTerminate();
      throw std::exception("Glfw window creation error!\n");
    }
    glfwMakeContextCurrent(window);
  } catch (const std::exception& error) {
    throw error;
  }

  m_mainWindow = window;
}

void WindowManager::initOpenGl() {
  GLenum err = glewInit();

  utl::log(utl::LGL_INFO, "Glew message: {}\n",
           (const char*)glewGetErrorString(err));

  lgl::displayOpenGlInfo();
  lgl::initializeDebugContext();

  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                        GL_TRUE);
  glDebugMessageCallback(&lgl::outputDebugMessage, (const void*)NULL);
}

void WindowManager::renderUserInterface() {
  glViewport(m_sceneWidth, 0, m_mainWindowWidth - m_sceneWidth,
             m_mainWindowHeight);
  static Rect rectangle(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f),
                        glm::vec3(0.2f, 0.2f, 0.3f));
  rectangle.draw();

  m_userInterface->renderUI();
}

WindowManager::WindowManager(const char* mainWindowName,
                             std::uint32_t mainWindowWidth,
                             std::uint32_t mainWindowHeight,
                             std::uint32_t sceneWidth,
                             std::uint32_t sceneHeight) {
  m_mainWindowName = mainWindowName;
  m_mainWindowWidth = mainWindowWidth;
  m_mainWindowHeight = mainWindowHeight;
  m_sceneWidth = sceneWidth;
  m_sceneHeight = sceneHeight;

  m_scene = nullptr;
  m_userInterface = nullptr;

  initMainWindow();
  initOpenGl();

  Mouse::initMouse(m_mainWindow);
  DebugDrawer::initProgram();
}

WindowManager::~WindowManager() {
  m_userInterface.reset();
  m_scene.reset();
  glfwDestroyWindow(m_mainWindow);
  glfwTerminate();
}

void WindowManager::renderWindowLoop() {
  while (!glfwWindowShouldClose(m_mainWindow)) {
    Time::calculateCurrentTime();

    if (Time::s_currentTime - Time::s_previousTime < Time::s_frameTime) {
      continue;
    }

    Time::calculateDeltaTime();

    // Logger::log(Logger::LGL_INFO, "Delta Time: {}\n", Time::s_deltaTime);

    glfwPollEvents();
    m_scene->update(m_mainWindow);

    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_sceneWidth, m_sceneHeight);
    m_scene->draw();

    renderUserInterface();

    glfwSwapBuffers(m_mainWindow);
  }
}
}  // namespace lgl