#pragma once

#include <gl/BaseDefs.hpp>
#include <gl/VertexBuffer.hpp>
#include <gl/VertexBufferLayout.hpp>

namespace lgl {
	class LGL_API VertexArray {

	private:
		UINT id;

	public:
		VertexArray();
		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) const;
		void bind() const;
		void unBind() const;
		~VertexArray();
	};
}