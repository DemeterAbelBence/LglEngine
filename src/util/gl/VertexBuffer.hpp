#pragma once

#include <GLEW/glew.h>

#include <gl/BaseDefs.hpp>

namespace lgl {

	class LGL_API VertexBuffer {

	private:
		UINT id;

	public:
		VertexBuffer();
		VertexBuffer(const void* data, UINT size);
		void setData(const void* data, UINT size);
		void bind() const;
		void unBind() const;
		~VertexBuffer();
	};
}