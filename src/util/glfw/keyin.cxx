#include "keyin.hxx"

namespace lgl {
    keyin::Key::Key() {
        m_id = 0;
        m_pressed = false;
    }

    keyin::Key::Key(utl::uint id) {
        m_id = id;
        m_pressed = false;
    }

    bool keyin::Key::handlePress(GLFWwindow* window) {
        if (getState(window) == GLFW_PRESS) {
            m_pressed = true;
            return true;
        }
        else {
            return false;
        }
    }

    bool keyin::Key::handleRelease(GLFWwindow* window) {
        if (getState(window) == GLFW_RELEASE && m_pressed) {
            m_pressed = false;
            return true;
        }
        else {
            return false;
        }
    }

    utl::uint keyin::Key::getState(GLFWwindow* window) {
        return glfwGetKey(window, m_id);
    }
}