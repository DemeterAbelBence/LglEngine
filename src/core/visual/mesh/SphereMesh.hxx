#pragma once

import utl;
import glm;

#include <visual/mesh/Surface.hxx>

namespace lgl {
class SphereMesh final : public Surface {
 private:
  float m_radius;

 public:
  SphereMesh(std::uint32_t N, std::uint32_t M, float radius);
  void eval(float u, float v, glm::vec3& pos, glm::vec3& norm) override;
};
}  // namespace lgl