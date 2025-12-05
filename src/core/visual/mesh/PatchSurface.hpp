#pragma once

#include <gl/BaseDefs.hpp>

#include <visual/mesh/Mesh.hpp>

namespace lgl {
	class LGL_API PatchSurface : public Mesh {

	private:
		UINT m_rezolution;
		float m_size;
		std::vector<glm::vec3> m_vertexData;

	public:
		PatchSurface(float size, UINT rezolution);
		void setSize(float size) { m_size = size; }
		void setRezolution(float rezolution) { m_rezolution = rezolution; }
		glm::vec3 generateVertexData(float x, float y);

		void create() override;
		void draw(const Camera& camera) const override;

	private:
		void setBufferData(const void* data, UINT size);
	};
}