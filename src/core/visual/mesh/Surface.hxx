#pragma once

import lglmodule;
import glmmodule;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
	class Surface : public Mesh {
	private:
		struct VertexData {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};

	private:
		utl::uint m_verticesPerStrip;
		utl::uint m_strips;
		float m_size;

		utl::vec<VertexData> m_vertexData;

	private:
		void setBufferData(const void* data, utl::uint size);

	public:
		Surface(utl::uint N, utl::uint M);
		void setSize(float size) { m_size = size; }

		VertexData generateVertexData(float x, float y);

		void create() override;
		void draw(const Camera& camera) const override;

		virtual void eval(float x, float y, glm::vec3& pos, glm::vec3& norm) = 0;
	};
}