#pragma once

import utl;
import glm;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
class Surface : public Mesh {
 private:
  struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
  };

 private:
  std::uint32_t m_verticesPerStrip;
  std::uint32_t m_strips;
  float m_size;

  std::vector<VertexData> m_vertexData;

 private:
  void setBufferData(const void* data, std::uint32_t size);

 public:
  Surface(std::uint32_t N, std::uint32_t M);
  void setSize(float size) { m_size = size; }

  VertexData generateVertexData(float x, float y);

  void create() override;
  void draw(const Camera& camera) const override;

  virtual void eval(float x, float y, glm::vec3& pos, glm::vec3& norm) = 0;
};
}  // namespace lgl