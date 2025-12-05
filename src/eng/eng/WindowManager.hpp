#pragma once

#include <GLEW/glew.h>

#include <gl/BaseDefs.hpp>
#include <gl/DebugDrawer.hpp>

#include <eng/Scene.hpp>
#include <eng/UserInterface.hpp>

namespace lgl {
	class LGL_API WindowManager {
	private:
		GLFWwindow* m_mainWindow;

		CSTRING m_mainWindowName;
		UINT m_mainWindowWidth;
		UINT m_mainWindowHeight;
		UINT m_sceneWidth;
		UINT m_sceneHeight;

		SPTR<Scene> m_scene;
		SPTR<UserInterface> m_userInterface;

	private:
		void initMainWindow();
		void initOpenGl();
		void renderUserInterface();

	public:
		WindowManager(CSTRING mainWindowName, UINT mainWindowWidth, 
			UINT mainWindowHeight, UINT sceneWidth, UINT sceneHeight);

		~WindowManager();

		template<typename U, typename V>
		void defineLayout() {
			try {
				m_scene = std::make_shared<U>();
				m_scene->create();
				m_userInterface = std::make_shared<V>(m_mainWindow, m_scene);
			}
			catch (const std::exception& error) {
				throw error;
			}
		}

		void renderWindowLoop();
	};
}