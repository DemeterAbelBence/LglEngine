#include "VertexBufferLayout.hxx"

namespace lgl {
	VertexBufferLayout::VertexBufferLayout() : m_stride(0) {}

	void VertexBufferLayout::push(utl::uint type, utl::uint count) {
		m_elements.push_back({ count, type, GL_FALSE });
		m_stride += VertexBufferElement::getSizeOfType(type) * count;
	}
}
