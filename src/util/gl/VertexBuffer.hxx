#pragma once

import lglmodule;

#include <GLEW/glew.h>

namespace lgl {
	class VertexBuffer {
	private:
		utl::uint m_id;

	public:
		VertexBuffer();
		VertexBuffer(utl::cmemptr data, utl::uint size);
		void setData(utl::cmemptr data, utl::uint size);
		void bind() const;
		void unBind() const;
		~VertexBuffer();
	};
}