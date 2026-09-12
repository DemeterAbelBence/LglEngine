#pragma once

import utl;
import glm;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
class PatchSurface : public Mesh {
 private:
  std::uint32_t m_rezolution;
  float m_size;
  std::vector<glm::vec3> m_vertexData;

 public:
  PatchSurface(float size, std::uint32_t rezolution);
  void setSize(float size) { m_size = size; }
  void setRezolution(std::uint32_t rezolution) { m_rezolution = rezolution; }
  glm::vec3 generateVertexData(float x, float y);

  void create() override;
  void draw(const Camera& camera) const override;

 private:
  void setBufferData(const void* data, std::uint32_t size);
};
}  // namespace lgl