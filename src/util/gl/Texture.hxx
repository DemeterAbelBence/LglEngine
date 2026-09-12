#pragma once

import std;
import utl;

#include <GLEW/glew.h>
#include <STB/stb_image.h>

namespace lgl {
struct Texture {
  std::uint32_t id;
  std::string name;
  std::string path;
};

namespace tex {
struct Texel {
  std::byte r;
  std::byte g;
  std::byte b;
  std::byte a;
};

std::uint32_t TextureFromFile(const std::string& path);
std::uint32_t TextureFromMemory(const std::byte* data, int nBytes);
std::uint32_t TextureFromAssimpTexels(const std::byte* data,
                                      std::uint32_t width,
                                      std::uint32_t height);
};  // namespace tex
}  // namespace lgl