#include "WindowManager.hxx"

namespace lgl {
    void WindowManager::initMainWindow() {
        GLFWwindow* window;
        try {
            if (!glfwInit()) {
                throw utl::except("Glfw initialization error!\n");
            }

            window = glfwCreateWindow(m_mainWindowWidth, m_mainWindowHeight, m_mainWindowName, NULL, NULL);
            if (!window) {
                glfwTerminate();
                throw utl::except("Glfw window creation error!\n");
            }
            glfwMakeContextCurrent(window);
        }
        catch (const utl::except& error) {
            throw error;
        }

        m_mainWindow = window;
    }

    void WindowManager::initOpenGl() {
        GLenum err = glewInit();

        Logger::log(Logger::LGL_INFO, "Glew message: {}\n", (utl::cstr)glewGetErrorString(err));

        lgl::displayOpenGlInfo();
        lgl::initializeDebugContext();

        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageCallback(&lgl::outputDebugMessage, (const void*)NULL);
    }

    void WindowManager::renderUserInterface() {
        glViewport(m_sceneWidth, 0, m_mainWindowWidth - m_sceneWidth, m_mainWindowHeight);
        static Rect rectangle(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.3f));
        rectangle.draw();

        m_userInterface->renderUI();
    }

    WindowManager::WindowManager(utl::cstr mainWindowName,
        utl::uint mainWindowWidth,
        utl::uint mainWindowHeight,
        utl::uint sceneWidth,
        utl::uint sceneHeight) {

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

            glfwPollEvents();
            glViewport(0, 0, m_sceneWidth, m_sceneHeight);
            glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_scene->update(m_mainWindow);
            m_scene->draw();

            renderUserInterface();

            glfwSwapBuffers(m_mainWindow);
        }
    }
}