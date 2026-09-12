#pragma once

import glm;

namespace lgl {
struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};
}  // namespace lgl