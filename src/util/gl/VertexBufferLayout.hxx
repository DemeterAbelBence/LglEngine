#pragma once

import std;

#include <GLEW/glew.h>

namespace lgl {
struct VertexBufferElement {
  std::uint32_t count;
  std::uint32_t type;
  std::uint32_t normalized;

  static std::uint32_t getSizeOfType(std::uint32_t type) {
    switch (type) {
      case GL_FLOAT:
        return 4;
    }
    return 0;
  }
};

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElement> m_elements;
  std::uint32_t m_stride;

 public:
  VertexBufferLayout();
  std::uint32_t getStride() const { return m_stride; }
  const std::vector<VertexBufferElement>& getElements() const {
    return m_elements;
  }
  void push(std::uint32_t type, std::uint32_t count);
};
}  // namespace lgl