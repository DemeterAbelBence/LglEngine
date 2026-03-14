#pragma once

import lglmodule;
import glmmodule;

#include <GLEW/glew.h>

#include <gl/DebugDrawer.hxx>
#include <eng/Scene.hxx>
#include <eng/UserInterface.hxx>

namespace lgl {
	class WindowManager {
	private:
		GLFWwindow* m_mainWindow;

		utl::cstr m_mainWindowName;
		utl::uint m_mainWindowWidth;
		utl::uint m_mainWindowHeight;
		utl::uint m_sceneWidth;
		utl::uint m_sceneHeight;

		utl::sptr<Scene> m_scene;
		utl::sptr<UserInterface> m_userInterface;

	private:
		void initMainWindow();
		void initOpenGl();
		void renderUserInterface();

	public:
		WindowManager(utl::cstr mainWindowName, utl::uint mainWindowWidth,
			utl::uint mainWindowHeight, utl::uint sceneWidth, utl::uint sceneHeight);

		~WindowManager();

		template<typename U, typename V>
		void defineLayout() {
			try {
				m_scene = utl::makeSptr<U>();
				m_scene->create();
				m_userInterface = utl::makeSptr<V>(m_mainWindow, m_scene);
			}
			catch (const utl::except& error) {
				throw error;
			}
		}

		void renderWindowLoop();
	};
}