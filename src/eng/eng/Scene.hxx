#pragma once

// LglMod
import utl;
import glm;

// LglUtil
#include <gl/FrameBuffer.hxx>
#include <gl/GpuProgram.hxx>
#include <glfw/Time.hxx>
#include <glfw/keyin.hxx>
#include <model/model.hxx>

// LglCore
#include <sceneobject/Box.hxx>
#include <sceneobject/Sphere.hxx>
#include <sceneobject/Sun.hxx>
#include <sceneobject/Terrain.hxx>
#include <visual/mesh/Mesh.hxx>

// LglEng
#include <eng/CollisionHandler.hxx>
#include <eng/ResourceRepository.hxx>

namespace lgl {
class Scene {
 protected:
  std::unique_ptr<Camera> m_camera;
  std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;
  std::unique_ptr<ResourceRepository> m_resources;

  keyin::KeyEventHandler<Camera, int> m_cameraEventHandler;
  keyin::KeyEventHandler<SceneObject, glm::vec3> m_objectEventHandler;

  std::uint32_t m_objectIndex;
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

  virtual ~Scene() = default;

  const std::unique_ptr<Camera>& getCamera() const { return m_camera; }
  void setMoveCamera(bool value) { m_moveCamera = value; }
  bool getMoveCamera() const { return m_moveCamera; }
  const std::vector<std::shared_ptr<SceneObject>>& getSceneObjects() const {
    return m_sceneObjects;
  }
  const std::shared_ptr<SceneObject>& getCurrentSceneObject() const {
    return m_sceneObjects[m_objectIndex];
  }

  void getNextObject();
  void setPrimitiveType(std::uint32_t type);
  void setCameraMoveSpeed(float speed);
  void setObjectMoveSpeed(float speed);
  void saveObjectStates(const std::string& path);
  void loadObjectStates(const std::string& path);

  virtual void readConfig(const utl::LglConf& lglConf) = 0;
  virtual void create() = 0;
  virtual void draw() const = 0;
  virtual void update(GLFWwindow* window) = 0;
};
}  // namespace lgl