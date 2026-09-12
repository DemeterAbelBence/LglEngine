#include "ResourceRepository.hxx"

void lgl::ResourceRepository::handleShaderChanges() {
  for (const auto& program : m_programs) {
    program.second->handleShaderSourceChange();
  }
}

std::shared_ptr<lgl::GpuProgram> lgl::ResourceRepository::getProgram(
    const char* name) {
  std::shared_ptr<GpuProgram> gpuProgram;
  try {
    gpuProgram = m_programs.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN,
             "GpuProgram resource name not in repository: ", name);
  }
  return gpuProgram;
}

std::shared_ptr<lgl::Material> lgl::ResourceRepository::getMaterial(
    const char* name) {
  std::shared_ptr<Material> material;
  try {
    material = m_materials.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN, "Material resource name not in repository: ", name);
  }
  return material;
}

std::shared_ptr<lgl::Texture> lgl::ResourceRepository::getTexture(
    const char* name) {
  std::shared_ptr<Texture> texture;
  try {
    texture = m_textures.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN, "Texture resource name not in repository: ", name);
  }
  return texture;
}

std::shared_ptr<lgl::Mesh> lgl::ResourceRepository::getMesh(const char* name) {
  std::shared_ptr<Mesh> mesh;
  try {
    mesh = m_meshes.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN, "Mesh resource name not in repository: ", name);
  }
  return mesh;
}

std::shared_ptr<lgl::ModelMesh> lgl::ResourceRepository::getModelMesh(
    const char* name) {
  std::shared_ptr<ModelMesh> modelMesh;
  try {
    modelMesh = m_modelMeshes.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN,
             "ModelMesh resource name not in repository: ", name);
  }
  return modelMesh;
}

std::shared_ptr<lgl::FrameBuffer> lgl::ResourceRepository::getBuffer(
    const char* name) {
  std::shared_ptr<FrameBuffer> buffer;
  try {
    buffer = m_buffers.at(name);
  } catch (const std::out_of_range&) {
    utl::log(utl::LGL_WARN,
             "FrameBuffer resource name not in repository: ", name);
  }
  return buffer;
}
