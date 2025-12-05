#pragma once

#include <GLEW/glew.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
	struct LGL_API VertexBufferElement {
		UINT count;
		UINT type;
		UINT normalized;

		static UINT getSizeOfType(UINT type) {
			switch (type) {
			case GL_FLOAT: return 4;
			}
			return 0;
		}
	};

	class LGL_API VertexBufferLayout {

	private:
		std::vector<VertexBufferElement> elements;
		UINT stride;

	public:
		VertexBufferLayout();
		UINT getStride() const { return stride; }
		const std::vector<VertexBufferElement>& getElements() const { return elements; }
		void push(UINT type, UINT count);
	};
}