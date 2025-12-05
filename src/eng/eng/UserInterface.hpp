#pragma once

#include <GLEW/glew.h>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <gl/BaseDefs.hpp>
#include <gl/Debug.hpp>

#include <visual/common/Rect.hpp>

#include <eng/Scene.hpp>



namespace lgl {
	class LGL_API UserInterface {
	protected:
		SPTR<Scene> m_scene;
		ImGuiIO* io;

	protected:
		void renderObjectPicker();
		void renderPrimitivePicker();
		void renderFrameRateBox();

		virtual void renderAdditionalUI();

	public:
		UserInterface(GLFWwindow* window, SPTR<Scene> scene);

		UserInterface(const UserInterface& other) = delete;
		UserInterface& operator=(const UserInterface& other) = delete;

		UserInterface(const UserInterface&& other) = delete;
		UserInterface& operator=(const UserInterface&& other) = delete;

		~UserInterface();

		void renderUI();
	};
}