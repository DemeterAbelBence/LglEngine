#pragma once

import utl;
import glm;

#include <collision/TerrainCollider.hxx>
#include <collision/TerrainData.hxx>
#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/PatchSurface.hxx>

namespace lgl {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class Terrain : public SceneObject {
 private:
  std::shared_ptr<TerrainData> m_terrainData;  ///< lorem ipsum

 private:
  void readTerrainTexturesPBR();

 public:
  Terrain(float size, std::uint32_t rezolution);

  void setParameters(float a, float f, float p, float i);
  void setTesselationParameters(std::uint32_t min_t, std::uint32_t max_t, float min_d, float max_d);
  void updateTerrainParameters();
};
}  // namespace lgl