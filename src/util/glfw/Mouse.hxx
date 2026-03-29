#pragma once

import lglmodule;

#include <GLFW/glfw3.h>

namespace lgl {
	struct MouseData {
		double lastX;
		double lastY;
		double offsetX;
		double offsetY;
	};

	class Mouse {
	private:
		static void mouseMoveCallback(GLFWwindow* window, double posX, double posY);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	public:
		inline static double s_sensitivity = 0.1;
		inline static bool s_isCaptured = false;
		inline static bool s_isLeftButtonPressed = false;

		inline static MouseData s_data = { 400.0, 400.0, 0.0, 0.0 };
		inline static GLFWwindow* s_window = nullptr;

		static void initMouse(GLFWwindow* window);
		static void toggleMouseCapture();
		static void resetOffsets();
	};
}