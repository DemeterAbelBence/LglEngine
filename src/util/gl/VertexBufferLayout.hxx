#pragma once

import lglmodule;

#include <GLEW/glew.h>

namespace lgl {
	struct VertexBufferElement {
		utl::uint count;
		utl::uint type;
		utl::uint normalized;

		static utl::uint getSizeOfType(utl::uint type) {
			switch (type) {
			case GL_FLOAT: return 4;
			}
			return 0;
		}
	};

	class VertexBufferLayout {
	private:
		utl::vec<VertexBufferElement> m_elements;
		utl::uint m_stride;

	public:
		VertexBufferLayout();
		utl::uint getStride() const { return m_stride; }
		const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
		void push(utl::uint type, utl::uint count);
	};
}