#include "CuboidMesh.hxx"

namespace lgl {
void CuboidMesh::createPoints() {
  m_points[0] = glm::vec3(-0.5f, -0.5f, 0.5f);
  m_points[1] = glm::vec3(0.5f, -0.5f, 0.5f);
  m_points[2] = glm::vec3(0.5f, -0.5f, -0.5f);
  m_points[3] = glm::vec3(-0.5f, -0.5f, -0.5f);

  m_points[4] = glm::vec3(-0.5f, 0.5f, 0.5f);
  m_points[5] = glm::vec3(0.5f, 0.5f, 0.5f);
  m_points[6] = glm::vec3(0.5f, 0.5f, -0.5f);
  m_points[7] = glm::vec3(-0.5f, 0.5f, -0.5f);
}

void CuboidMesh::createSide(const std::array<std::uint32_t, 4>& indices) {
  glm::vec3 sideCenter = glm::vec3(0.0f);
  for (std::uint32_t i : indices) {
    sideCenter += m_points[i];
  }
  sideCenter /= static_cast<float>(indices.size());

  glm::vec3 normal = sideCenter - calculateCenter();

  m_vertexData.push_back({m_points[indices[0]], normal, glm::vec2(0.0f, 0.0f)});
  m_vertexData.push_back({m_points[indices[1]], normal, glm::vec2(0.0f, 1.0f)});
  m_vertexData.push_back({m_points[indices[2]], normal, glm::vec2(1.0f, 1.0f)});

  m_vertexData.push_back({m_points[indices[2]], normal, glm::vec2(1.0f, 1.0f)});
  m_vertexData.push_back({m_points[indices[3]], normal, glm::vec2(1.0f, 0.0f)});
  m_vertexData.push_back({m_points[indices[0]], normal, glm::vec2(0.0f, 0.0f)});
}

void CuboidMesh::setBufferData(const void* data, std::uint32_t size) {
  m_vertexBuffer->setData(data, size);
  m_vertexBufferLayout->push(GL_FLOAT, 3);
  m_vertexBufferLayout->push(GL_FLOAT, 3);
  m_vertexBufferLayout->push(GL_FLOAT, 2);

  m_vertexArray->addBuffer(*m_vertexBuffer, *m_vertexBufferLayout);
}

void CuboidMesh::setResources(
    std::shared_ptr<GpuProgram> program, std::shared_ptr<Material> material,
    const std::vector<std::shared_ptr<Texture>>& textures) {
  m_program = program;
  m_material = material;
  for (auto texture : textures) {
    addTexture(texture);
  }
}

CuboidMesh::CuboidMesh() : Mesh() { create(); }

glm::vec3 CuboidMesh::calculateCenter() const {
  glm::vec3 result = glm::vec3(0.0f);

  for (glm::vec3 p : m_points) result += p;

  return result / static_cast<float>(m_points.size());
}

void CuboidMesh::create() {
  createPoints();

  createSide({0, 1, 2, 3});
  createSide({4, 5, 6, 7});

  createSide({0, 3, 7, 4});
  createSide({1, 2, 6, 5});

  createSide({0, 1, 5, 4});
  createSide({3, 2, 6, 7});

  setBufferData((const void*)&m_vertexData[0],
                m_vertexData.size() * sizeof(VertexData));
}

void CuboidMesh::draw(const Camera& camera) const {
  m_vertexArray->bind();

  m_program->bind();
  m_program->setMat4("MI", m_transformation->makeModelInverseMatrix());
  m_program->setMat4("M", m_transformation->makeModelMatrix());
  m_program->setMat4("V", camera.getV());
  m_program->setMat4("P", camera.getP());
  m_program->setVec3("eye", camera.getEye());
  setAllUniforms();

  glPolygonMode(GL_FRONT_AND_BACK, m_primitiveType);
  glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size());
}
}  // namespace lgl