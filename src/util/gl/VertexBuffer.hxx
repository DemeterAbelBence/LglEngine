#pragma once

import std;

#include <GLEW/glew.h>

namespace lgl {
	class VertexBuffer {
	private:
		std::uint32_t m_id;

	public:
		VertexBuffer();
		VertexBuffer(const void* data, std::uint32_t size);
		void setData(const void* data, std::uint32_t size);
		void bind() const;
		void unBind() const;
		~VertexBuffer();
	};
}