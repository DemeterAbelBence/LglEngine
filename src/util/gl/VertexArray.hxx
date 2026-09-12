#pragma once

import std;

#include <GLEW/glew.h>

#include <gl/VertexBuffer.hxx>
#include <gl/VertexBufferLayout.hxx>

namespace lgl {
	class VertexArray {
	private:
		std::uint32_t m_id;

	public:
		VertexArray();
		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) const;
		void bind() const;
		void unBind() const;
		~VertexArray();
	};
}