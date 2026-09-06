#pragma once

// LglMod
import lglmodule;
import glmmodule;

// LglUtil 
#include <gl/GpuProgram.hxx>
#include <gl/FrameBuffer.hxx>
#include <glfw/keyin.hxx>
#include <glfw/Time.hxx>
#include <model/model.hxx>
		 
// LglCore
#include <sceneobject/Box.hxx>
#include <sceneobject/Sun.hxx>
#include <sceneobject/Terrain.hxx>
#include <sceneobject/Sphere.hxx> 
#include <visual/mesh/Mesh.hxx>
	
// LglEng
#include <eng/CollisionHandler.hxx>
#include <eng/ResourceRepository.hxx>

namespace lgl {
	class Scene {
	protected:
		utl::uptr<Camera> m_camera;
		utl::svec<SceneObject> m_sceneObjects;
		utl::uptr<ResourceRepository> m_resources;

		keyin::KeyEventHandler<Camera, int> m_cameraEventHandler;
		keyin::KeyEventHandler<SceneObject, glm::vec3> m_objectEventHandler;

		utl::uint m_objectIndex;
		bool m_moveCamera;

	protected:
		void initObjectEvents();
		void initCameraEvents();

	public:
		Scene();

		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene& other) = delete;

		Scene(const Scene&& other) = delete;
		Scene& operator=(const Scene&& other) = delete;

		const utl::uptr<Camera>& getCamera() const { return m_camera; }
		void setMoveCamera(bool value) { m_moveCamera = value; }
		bool getMoveCamera() const { return m_moveCamera; }
		const utl::svec<SceneObject>& getSceneObjects() const { return m_sceneObjects; }
		const utl::sptr<SceneObject>& getCurrentSceneObject() const { return m_sceneObjects[m_objectIndex]; }

		void getNextObject();
		void setPrimitiveType(utl::uint type);
		void setCameraMoveSpeed(float speed);
		void setObjectMoveSpeed(float speed);
		void saveObjectStates(utl::str path);
		void loadObjectStates(utl::str path);

		virtual void create() = 0;
		virtual void draw() const = 0;
		virtual void update(GLFWwindow* window) = 0;
	};
}