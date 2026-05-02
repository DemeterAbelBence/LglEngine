#pragma once

import lglmodule;
import glmmodule;

#include <GLEW/glew.h>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <gl/Debug.hxx>
#include <visual/common/Rect.hxx>
#include <eng/Scene.hxx>

namespace lgl {
	class UserInterface {
	protected:
		utl::sptr<Scene> m_scene;
		ImGuiIO* io;

	protected:
		void renderObjectPicker();
		void renderPrimitivePicker();
		void renderFrameRateBox();
		void renderSceneObjectLabels();

		virtual void renderAdditionalUI();

	public:
		UserInterface(GLFWwindow* window, utl::sptr<Scene> scene);

		UserInterface(const UserInterface& other) = delete;
		UserInterface& operator=(const UserInterface& other) = delete;

		UserInterface(const UserInterface&& other) = delete;
		UserInterface& operator=(const UserInterface&& other) = delete;

		~UserInterface();

		void renderUI();
	};
}