#pragma once

#include <gl/BaseDefs.hpp>
#include <gl/GpuProgram.hpp>
#include <gl/FrameBuffer.hpp>

#include <glfw/keyin.hpp>
#include <glfw/Time.hpp>

#include <asim/asim.hpp>
		 
#include <sceneobject/Box.hpp>
#include <sceneobject/Sun.hpp>
#include <sceneobject/Terrain.hpp>
#include <sceneobject/Sphere.hpp>
#include <sceneobject/Sponza.hpp>
		 
#include <visual/mesh/Mesh.hpp>
		 
#include <eng/CollisionHandler.hpp>

namespace lgl {
	class LGL_API Scene {
	protected:
		UPTR<Camera> m_camera;
		TVEC<SceneObject> m_sceneObjects;
		TMAP<GpuProgram> m_programs;
		TMAP<Material> m_materials;
		TMAP<Texture> m_textures;
		TMAP<Mesh> m_meshes;
		TMAP<ModelMesh> m_modelMeshes;
		TMAP<FrameBuffer> m_buffers;

		keyin::KeyEventHandler<Camera, int> m_cameraEventHandler;
		keyin::KeyEventHandler<SceneObject, glm::vec3> m_objectEventHandler;

		UINT m_objectIndex;
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

		const UPTR<Camera>& getCamera() const { return m_camera; }
		void setMoveCamera(bool value) { m_moveCamera = value; }
		bool getMoveCamera() const { return m_moveCamera; }
		const SPTR<GpuProgram>& getProgram(CSTRING name) const { return m_programs.at(name); }
		const TVEC<SceneObject>& getSceneObjects() const { return m_sceneObjects; }
		const SPTR<SceneObject>& getCurrentSceneObject() const { return m_sceneObjects[m_objectIndex]; }

		void getNextObject();
		void setPrimitiveType(UINT type);
		void setCameraMoveSpeed(float speed);
		void setObjectMoveSpeed(float speed);

		virtual void create() = 0;
		virtual void draw() const = 0;
		virtual void update(GLFWwindow* window) = 0;
	};
}