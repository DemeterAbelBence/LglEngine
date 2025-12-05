#pragma once 

#include <GLEW/glew.h>
#include <STB/stb_image.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
	struct LGL_API Texture {
		UINT id;
		std::string name;
		std::string path;
	};

	namespace tex {
		struct LGL_API Texel {
			BYTE r;
			BYTE g;
			BYTE b;
			BYTE a;
		};

		LGL_API UINT TextureFromFile(CSTRING path);
		LGL_API UINT TextureFromMemory(BYTE* data, int nBytes);
		LGL_API UINT TextureFromAssimpTexels(BYTE* data, UINT width, UINT height);
	};
}