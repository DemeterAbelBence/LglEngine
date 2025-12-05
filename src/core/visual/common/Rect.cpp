#include "Rect.hpp"

using namespace lgl;

Rect::Rect(glm::vec2 corner1, glm::vec2 corner2, glm::vec3 color) {
	m_color = color;

	m_vertexArray = std::make_unique<VertexArray>();
	m_vertexBuffer = std::make_unique<VertexBuffer>();
	m_bufferLayout = std::make_unique<VertexBufferLayout>();

	m_shader = std::make_unique<GpuProgram>("Rectangle Shader");
	m_shader->addShader(GL_FRAGMENT_SHADER, "shaders/basic/rect-frag.glsl");
	m_shader->addShader(GL_VERTEX_SHADER, "shaders/basic/rect-vert.glsl");
	m_shader->createProgram();
	m_shader->bind();
	m_shader->setVec3("rectColor", color);

	glm::vec2 corner3;
	corner3.x = corner1.x;
	corner3.y = corner2.y;

	glm::vec2 corner4;
	corner4.x = corner2.x;
	corner4.y = corner1.y;

	std::vector<glm::vec2> vertexData;
	vertexData.push_back(corner1);
	vertexData.push_back(corner2);
	vertexData.push_back(corner3);

	vertexData.push_back(corner4);
	vertexData.push_back(corner2);
	vertexData.push_back(corner1);

	m_vertexBuffer->setData((const void*)&vertexData[0], vertexData.size() * sizeof(glm::vec2));
	m_bufferLayout->push(GL_FLOAT, 2);
	m_vertexArray->addBuffer(*m_vertexBuffer, *m_bufferLayout);
}

void Rect::draw() const {
	m_vertexArray->bind();
	m_shader->bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
