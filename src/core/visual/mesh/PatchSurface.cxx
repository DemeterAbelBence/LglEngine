#include "PatchSurface.hxx"

namespace lgl {
	PatchSurface::PatchSurface(float size, utl::uint rezolution) {
		m_rezolution = rezolution;
		m_size = size;
	}

	glm::vec3 PatchSurface::generateVertexData(float x, float y) {
		glm::vec3 position = glm::vec3(x, 0.0f, y);
		return position;
	}

	void PatchSurface::create() {
		utl::uint r = m_rezolution;
		float s = m_size;

		for (utl::uint i = 0; i < r; i++) {
			for (utl::uint j = 0; j < r; j++) {
				m_vertexData.push_back(generateVertexData(s * (float)i / r, s * (float)j / r));
				m_vertexData.push_back(generateVertexData(s * (float)(i + 1) / r, s * (float)j / r));
				m_vertexData.push_back(generateVertexData(s * (float)i / r, s * (float)(j + 1) / r));
				m_vertexData.push_back(generateVertexData(s * (float)(i + 1) / r, s * (float)(j + 1) / r));
			}
		}
		setBufferData((utl::cmemptr)&m_vertexData[0], m_vertexData.size() * sizeof(glm::vec3));
	}

	void PatchSurface::draw(const Camera& camera) const {
		m_vertexArray->bind();

		m_program->bind();
		m_program->setMat4("MI", m_transformation->makeModelInverseMatrix());
		m_program->setMat4("M", m_transformation->makeModelMatrix());
		m_program->setMat4("V", camera.getV());
		m_program->setMat4("P", camera.getP());
		m_program->setVec3("eye", camera.getEye());
		setAllUniforms();

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, m_primitiveType);
		glDrawArrays(GL_PATCHES, 0, 4 * m_rezolution * m_rezolution);
	}

	void PatchSurface::setBufferData(utl::cmemptr data, utl::uint size) {
		m_vertexBuffer->setData(data, size);
		m_vertexBufferLayout->push(GL_FLOAT, 3);
		m_vertexArray->addBuffer(*m_vertexBuffer, *m_vertexBufferLayout);
	}
}