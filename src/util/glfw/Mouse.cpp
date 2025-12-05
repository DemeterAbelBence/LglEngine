#include "Mouse.hpp"

using namespace lgl;

double Mouse::s_sensitivity = 0.1;

Mouse::MouseData Mouse::s_data = { 400.0, 400.0, 0.0, 0.0 };

bool Mouse::s_isCaptured = false;

bool Mouse::s_isLeftButtonPressed = false;

GLFWwindow* Mouse::s_window = nullptr;

void Mouse::mouseMoveCallback(GLFWwindow* window, double posX, double posY) {
	s_data.offsetX = posX - s_data.lastX;
	s_data.offsetY = posY - s_data.lastY;
	s_data.lastX = posX;
	s_data.lastY = posY;

	s_data.offsetX *= s_sensitivity;
	s_data.offsetY *= s_sensitivity;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		s_isLeftButtonPressed = true;
	}
	else {
		s_isLeftButtonPressed = false;
	}
}

void Mouse::initMouse(GLFWwindow* window) {
	s_window = window;

	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPos(window, 400.0, 400.0);
}

void Mouse::toggleMouseCapture() {
	if (s_isCaptured) {
		glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		s_isCaptured = false;
	}
	else {
		glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		s_isCaptured = true;
	}
}
