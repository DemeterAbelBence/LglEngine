#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
	class LGL_API Mouse {
	public:
		struct MouseData {
			double lastX;
			double lastY;
			double offsetX;
			double offsetY;
		};

	private:
		static void mouseMoveCallback(GLFWwindow* window, double posX, double posY);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	public:
		static double s_sensitivity;
		static bool s_isCaptured;
		static bool s_isLeftButtonPressed;

		static MouseData s_data;
		static GLFWwindow* s_window;

		static void initMouse(GLFWwindow* window);
		static void toggleMouseCapture();
	};
}