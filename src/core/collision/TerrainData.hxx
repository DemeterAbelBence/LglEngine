#pragma once

import lglmodule;

namespace lgl {
	struct TerrainData {
		float amplitude;
		float frequency;
		float phase;
		utl::uint iterations;

		utl::uint minimal_tesselation = 0;
		utl::uint maximal_tesselation = 0;

		float minimal_distance = 0.0f;
		float maximal_distance = 0.0f;
	};
}