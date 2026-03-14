#pragma once

import lglmodule;
import glmmodule;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
	class PatchSurface : public Mesh {

	private:
		utl::uint m_rezolution;
		float m_size;
		utl::vec<glm::vec3> m_vertexData;

	public:
		PatchSurface(float size, utl::uint rezolution);
		void setSize(float size) { m_size = size; }
		void setRezolution(float rezolution) { m_rezolution = rezolution; }
		glm::vec3 generateVertexData(float x, float y);

		void create() override;
		void draw(const Camera& camera) const override;

	private:
		void setBufferData(utl::cmemptr data, utl::uint size);
	};
}