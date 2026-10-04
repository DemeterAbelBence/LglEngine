#include "DebugDrawer.hxx"

namespace lgl {
void DebugDrawer::initProgram() {
  s_program = std::make_shared<GpuProgram>("DEBUG SHADER");
  s_program->addShader(GL_FRAGMENT_SHADER, std::format("{}/shaders/basic/debug-frag.glsl", utl::lglResourceFolder));
  s_program->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/basic/debug-vert.glsl", utl::lglResourceFolder));
  s_program->createProgram();
}

void DebugDrawer::setVertexData(const std::vector<glm::vec3>& vertexData) {
  s_vertexData.clear();
  s_vertexData = vertexData;
}

void DebugDrawer::draw(glm::mat4 V, glm::mat4 P, glm::vec3 color) {
  if (s_vertexData.size() == 0) {
    return;
  }

  s_program->bind();
  s_program->setMat4("V", V);
  s_program->setMat4("P", P);
  s_program->setVec3("color", color);
  s_program->setInt("overrideZ", s_overrideZ);

  std::uint32_t vertexArrayId;
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  std::uint32_t vertexBufferId;
  glGenBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, s_vertexData.size() * sizeof(glm::vec3), (const void*)&s_vertexData[0],
               GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);

  if (s_mode == GL_POINTS) {
    glPointSize(10.0f);
  }
  if (s_mode == GL_LINES) {
    glLineWidth(2.0f);
  }

  glDrawArrays(s_mode, 0, s_vertexData.size());

  glDeleteBuffers(1, &vertexBufferId);
  glDeleteVertexArrays(1, &vertexArrayId);
}
}  // namespace lgl