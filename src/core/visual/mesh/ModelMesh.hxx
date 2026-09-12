#pragma once

import utl;
import glm;

#include <model/model.hxx>
#include <visual/mesh/Mesh.hxx>

namespace lgl {
class ModelMesh : public Mesh {
 private:
  std::unique_ptr<model::AssimpModel> m_assimpModel;

 public:
  ModelMesh(const std::string& filePath, bool centerMeshes, bool loadTextures);
  void scaleModelBy(const glm::vec3& scaling);
  void create() override;
  void draw(const Camera& camera) const override;
  ~ModelMesh();
};
}  // namespace lgl
