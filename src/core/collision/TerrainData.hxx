#pragma once

import std;

namespace lgl {
	struct TerrainData {
		float amplitude;
		float frequency;
		float phase;
		std::uint32_t iterations;

		std::uint32_t minimal_tesselation = 0;
		std::uint32_t maximal_tesselation = 0;

		float minimal_distance = 0.0f;
		float maximal_distance = 0.0f;
	};
}