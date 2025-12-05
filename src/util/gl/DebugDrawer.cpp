#include "DebugDrawer.hpp"

using namespace lgl;

std::shared_ptr<GpuProgram> lgl::DebugDrawer::m_program = std::make_shared<GpuProgram>();
UINT DebugDrawer::m_mode = GL_POINTS;
UINT DebugDrawer::m_overrideZ = 0;

std::vector<glm::vec3> DebugDrawer::m_vertexData = std::vector<glm::vec3>();

void DebugDrawer::initProgram() {
	m_program = std::make_shared<GpuProgram>("DEBUG SHADER");
	m_program->addShader(GL_FRAGMENT_SHADER, "shaders/basic/debug-frag.glsl");
	m_program->addShader(GL_VERTEX_SHADER, "shaders/basic/debug-vert.glsl");
	m_program->createProgram();
}

void DebugDrawer::setVertexData(const std::vector<glm::vec3>& _vertexData) {
	m_vertexData.clear();
	m_vertexData = _vertexData;
}

void DebugDrawer::draw(const Camera& camera, glm::vec3 color) {
	if (m_vertexData.size() == 0) {
		return;
	}

	m_program->bind();
	m_program->setMat4("V", camera.getView());
	m_program->setMat4("P", camera.getProjection());
	m_program->setVec3("color", color);
	m_program->setInt("overrideZ", m_overrideZ);

	UINT vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	UINT vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(glm::vec3), (const void*)&m_vertexData[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);

	if (m_mode == GL_POINTS) {
		glPointSize(10.0f);
	}
	if(m_mode == GL_LINES) {
		glLineWidth(2.0f);
	}

	glDrawArrays(m_mode, 0, m_vertexData.size());

	glDeleteBuffers(1, &vertexBufferId);
	glDeleteVertexArrays(1, &vertexArrayId);
}
