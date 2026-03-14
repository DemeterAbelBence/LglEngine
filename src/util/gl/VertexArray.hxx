#pragma once

import lglmodule;

#include <GLEW/glew.h>

#include <gl/VertexBuffer.hxx>
#include <gl/VertexBufferLayout.hxx>

namespace lgl {
	class VertexArray {
	private:
		utl::uint m_id;

	public:
		VertexArray();
		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) const;
		void bind() const;
		void unBind() const;
		~VertexArray();
	};
}