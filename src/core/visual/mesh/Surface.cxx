#include "Surface.hxx"

namespace lgl {
	Surface::Surface(utl::uint N, utl::uint M) : Mesh() {
		m_vertexData = utl::vec<VertexData>();

		m_verticesPerStrip = (M + 1) * 2;
		m_strips = N;
		m_size = 1.0f;
	}

	Surface::VertexData Surface::generateVertexData(float x, float y) {
		glm::vec3 position;
		glm::vec3 normal;
		eval(x, y, position, normal);

		return { position, normal, glm::vec2(x, y) };
	}

	void Surface::create() {
		utl::uint N = m_strips;
		utl::uint M = m_verticesPerStrip / 2 - 1;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j <= M; j++) {
				m_vertexData.push_back(generateVertexData(m_size * (float)j / M, m_size * (float)i / N));
				m_vertexData.push_back(generateVertexData(m_size * (float)j / M, m_size * (float)(i + 1) / N));
			}
		}
		setBufferData((utl::cmemptr)&m_vertexData[0], m_vertexData.size() * sizeof(VertexData));
	}

	void Surface::draw(const Camera& camera) const {
		m_vertexArray->bind();

		m_program->bind();
		m_program->setMat4("MI", m_transformation->makeModelInverseMatrix());
		m_program->setMat4("M", m_transformation->makeModelMatrix());
		m_program->setMat4("V", camera.getView());
		m_program->setMat4("P", camera.getProjection());
		m_program->setVec3("eye", camera.getEye());
		setAllUniforms();

		glPolygonMode(GL_FRONT_AND_BACK, m_primitiveType);

		for (utl::uint i = 0; i < 4 * m_strips; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, i * m_verticesPerStrip, m_verticesPerStrip);
	}

	void Surface::setBufferData(utl::cmemptr data, utl::uint size) {
		m_vertexBuffer->setData(data, size);
		m_vertexBufferLayout->push(GL_FLOAT, 3);
		m_vertexBufferLayout->push(GL_FLOAT, 3);
		m_vertexBufferLayout->push(GL_FLOAT, 2);

		m_vertexArray->addBuffer(*m_vertexBuffer, *m_vertexBufferLayout);
	}
}