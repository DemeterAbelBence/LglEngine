#pragma once

import utl;

#include <gl/FrameBuffer.hxx>
#include <gl/GpuProgram.hxx>
#include <model/model.hxx>
#include <visual/common/Material.hxx>
#include <visual/mesh/Mesh.hxx>
#include <visual/mesh/ModelMesh.hxx>

namespace lgl {
class ResourceRepository {
 protected:
  std::unordered_map<std::string, std::shared_ptr<GpuProgram>> m_programs;
  std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
  std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
  std::unordered_map<std::string, std::shared_ptr<ModelMesh>> m_modelMeshes;
  std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> m_buffers;

 public:
  ResourceRepository() = default;

  ResourceRepository(const ResourceRepository& other) = delete;
  ResourceRepository& operator=(const ResourceRepository& other) = delete;

  ResourceRepository(const ResourceRepository&& other) = delete;
  ResourceRepository& operator=(const ResourceRepository&& other) = delete;

  std::shared_ptr<GpuProgram> getProgram(const char* name);
  std::shared_ptr<Material> getMaterial(const char* name);
  std::shared_ptr<Texture> getTexture(const char* name);
  std::shared_ptr<Mesh> getMesh(const char* name);
  std::shared_ptr<ModelMesh> getModelMesh(const char* name);
  std::shared_ptr<FrameBuffer> getBuffer(const char* name);

  void handleShaderChanges();

 protected:
  virtual void initializeResources() = 0;
};
}  // namespace lgl
