#pragma once

#include <sceneobject/SceneObject.hpp>

#include <visual/mesh/PatchSurface.hpp>

#include <collision/TerrainCollider.hpp>
#include <collision/TerrainData.hpp>

namespace lgl {
	class LGL_API Terrain : public SceneObject {
	private:
		std::unique_ptr<TerrainData> m_terrainData;

	private:
		void readTerrainTexturesPBR();

	public:
		Terrain(float size, UINT rezolution);

		void setParameters(float a, float f, float p, float i);
		void setTesselationParameters(UINT min_t, UINT max_t, float min_d, float max_d);
		void updateTerrainParameters();
	};
}