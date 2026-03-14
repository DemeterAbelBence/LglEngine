#pragma once

import lglmodule;
import glmmodule;

#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/PatchSurface.hxx>
#include <collision/TerrainCollider.hxx>
#include <collision/TerrainData.hxx>

namespace lgl {
	class Terrain : public SceneObject {
	private:
		utl::uptr<TerrainData> m_terrainData;

	private:
		void readTerrainTexturesPBR();

	public:
		Terrain(float size, utl::uint rezolution);

		void setParameters(float a, float f, float p, float i);
		void setTesselationParameters(utl::uint min_t, utl::uint max_t, float min_d, float max_d);
		void updateTerrainParameters();
	};
}