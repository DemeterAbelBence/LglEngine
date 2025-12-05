#include "Mesh.hpp"

using namespace lgl;

void Mesh::drawMeshFrame(const Camera& camera) {
	glm::vec3 s = m_transformation->getScaling();
	m_transformation->setScaleMatrix(s + 0.15f * s); 
	m_meshFrame = 1;
	draw(camera);

	m_transformation->setScaleMatrix(s);
	m_meshFrame = 0;
}

Mesh::Mesh() {
	m_vertexArray = std::make_unique<VertexArray>();
	m_vertexBuffer = std::make_unique<VertexBuffer>();
	m_vertexBufferLayout = std::make_unique<VertexBufferLayout>();

	m_meshFrame = 0;
	m_primitiveType = GL_TRIANGLES;
}

void Mesh::setAllUniforms() const {
	if (m_program) {
		m_program->bind();
		if (m_material) {
			m_program->setVec3("material.ambient", m_material->ambient);
			m_program->setVec3("material.diffuse", m_material->diffuse);
			m_program->setVec3("material.specular", m_material->specular);
			m_program->setFloat("material.shininess", m_material->shininess);
		}
		if (!m_textures.empty()) {
			for (UINT i = 0; i < m_textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				m_program->setInt(m_textures[i]->name.c_str(), i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i]->id);
			}
			glActiveTexture(GL_TEXTURE0);
		}
		m_program->setInt("meshFrame", m_meshFrame);
	}
}

Mesh::~Mesh() {}
