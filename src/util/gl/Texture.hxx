#pragma once 

import lglmodule;

#include <GLEW/glew.h>
#include <STB/stb_image.h>

namespace lgl {
	struct Texture {
		utl::uint id;
		utl::str name;
		utl::str path;
	};

	namespace tex {
		struct Texel {
			utl::byte r;
			utl::byte g;
			utl::byte b;
			utl::byte a;
		};

		utl::uint TextureFromFile(utl::cstr path);
		utl::uint TextureFromMemory(utl::byte* data, int nBytes);
		utl::uint TextureFromAssimpTexels(utl::byte* data, utl::uint width, utl::uint height);
	};
}