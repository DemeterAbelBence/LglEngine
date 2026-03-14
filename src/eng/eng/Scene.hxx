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

namespace lgl {
	class Scene {
	protected:
		utl::uptr<Camera> m_camera;
		utl::svec<SceneObject> m_sceneObjects;
		utl::smap<GpuProgram> m_programs;
		utl::smap<Material> m_materials;
		utl::smap<Texture> m_textures;
		utl::smap<Mesh> m_meshes;
		utl::smap<ModelMesh> m_modelMeshes;
		utl::smap<FrameBuffer> m_buffers;

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
		const utl::sptr<GpuProgram>& getProgram(utl::cstr name) const { return m_programs.at(name); }
		const utl::svec<SceneObject>& getSceneObjects() const { return m_sceneObjects; }
		const utl::sptr<SceneObject>& getCurrentSceneObject() const { return m_sceneObjects[m_objectIndex]; }

		void getNextObject();
		void setPrimitiveType(utl::uint type);
		void setCameraMoveSpeed(float speed);
		void setObjectMoveSpeed(float speed);

		virtual void create() = 0;
		virtual void draw() const = 0;
		virtual void update(GLFWwindow* window) = 0;
	};
}