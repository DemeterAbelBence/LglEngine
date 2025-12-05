#pragma once

#include <gl/BaseDefs.hpp>

#include <visual/mesh/Mesh.hpp>

namespace lgl {
	class LGL_API Surface : public Mesh {
	private:
		struct VertexData {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 tex_coord;
		};

	private:
		UINT m_verticesPerStrip;
		UINT m_strips;
		float m_size;

		std::vector<VertexData> m_vertexData;

	private:
		void setBufferData(const void* data, UINT size);

	public:
		Surface(UINT N, UINT M);
		void setSize(float size) { m_size = size; }

		VertexData generateVertexData(float x, float y);

		void create() override;
		void draw(const Camera& camera) const override;

		virtual void eval(float x, float y, glm::vec3& pos, glm::vec3& norm) = 0;
	};
}