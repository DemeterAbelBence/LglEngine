#pragma once

import utl;
import std;

#include <GLEW/glew.h>

#include <gl/GpuProgram.hxx>
#include <gl/VertexArray.hxx>

namespace lgl {
class DebugDrawer {
 private:
  inline static std::shared_ptr<GpuProgram> s_program = std::make_shared<GpuProgram>();
  inline static std::vector<glm::vec3> s_vertexData = std::vector<glm::vec3>();
  inline static std::uint32_t s_mode = GL_LINES;
  inline static std::uint32_t s_overrideZ = 0;

 public:
  static void initProgram();
  static void setMode(std::uint32_t mode) { s_mode = mode; }
  static void setOverrideZ(std::uint32_t overrideZ) { s_overrideZ = overrideZ; }
  static void setVertexData(const std::vector<glm::vec3>& vertexData);
  static void draw(glm::mat4 V, glm::mat4 P, glm::vec3 color);
};
}  // namespace lgl