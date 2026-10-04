#pragma once

import utl;
import glm;
import std;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
class CuboidMesh final : public Mesh {
 private:
  struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinate;
  };
  std::vector<VertexData> m_vertexData;

 private:
  std::array<glm::vec3, 8> m_points;

 private:
  void createPoints();
  void createSide(const std::array<std::uint32_t, 4>& indices);
  void setBufferData(const void* data, std::uint32_t size);

 public:
  CuboidMesh();
  glm::vec3 calculateCenter() const;
  void setResources(std::shared_ptr<GpuProgram> program, std::shared_ptr<Material> material,
                    const std::vector<std::shared_ptr<Texture>>& textures);

  void create() override;
  void draw(const Camera& camera) const override;
};
}  // namespace lgl