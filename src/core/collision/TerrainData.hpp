#pragma once

namespace lgl {
	struct TerrainData {
		float amplitude;
		float frequency;
		float phase;
		UINT iterations;

		UINT minimal_tesselation = 0;
		UINT maximal_tesselation = 0;

		float minimal_distance = 0.0f;
		float maximal_distance = 0.0f;
	};
}