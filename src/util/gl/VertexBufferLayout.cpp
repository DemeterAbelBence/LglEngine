#pragma once

#include "VertexBufferLayout.hpp"

using namespace lgl;

VertexBufferLayout::VertexBufferLayout() : stride(0) {}

void VertexBufferLayout::push(UINT type, UINT count) {
	elements.push_back({ count, type, GL_FALSE });
	stride += VertexBufferElement::getSizeOfType(type) * count;
}
