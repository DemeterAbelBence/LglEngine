#include "DebugDrawer.hxx"

namespace lgl {
	void DebugDrawer::initProgram() {
		m_program = utl::makeSptr<GpuProgram>("DEBUG SHADER");
		m_program->addShader(GL_FRAGMENT_SHADER, "shaders/basic/debug-frag.glsl");
		m_program->addShader(GL_VERTEX_SHADER, "shaders/basic/debug-vert.glsl");
		m_program->createProgram();
	}

	void DebugDrawer::setVertexData(const utl::vec<glm::vec3>& vertexData) {
		m_vertexData.clear();
		m_vertexData = vertexData;
	}

	void DebugDrawer::draw(glm::mat4 V, glm::mat4 P, glm::vec3 color) {
		if (m_vertexData.size() == 0) {
			return;
		}

		m_program->bind();
		m_program->setMat4("V", V);
		m_program->setMat4("P", P);
		m_program->setVec3("color", color);
		m_program->setInt("overrideZ", m_overrideZ);

		utl::uint vertexArrayId;
		glGenVertexArrays(1, &vertexArrayId);
		glBindVertexArray(vertexArrayId);

		utl::uint vertexBufferId;
		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(glm::vec3), (const void*)&m_vertexData[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);

		if (m_mode == GL_POINTS) {
			glPointSize(10.0f);
		}
		if (m_mode == GL_LINES) {
			glLineWidth(2.0f);
		}

		glDrawArrays(m_mode, 0, m_vertexData.size());

		glDeleteBuffers(1, &vertexBufferId);
		glDeleteVertexArrays(1, &vertexArrayId);
	}
}