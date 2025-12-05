#include "ModelMesh.hpp"

using namespace lgl;

ModelMesh::ModelMesh(const std::string& filePath, bool centerMeshes, bool loadTextures) : Mesh() {
	m_assimpModel = std::make_unique<asim::AssimpModel>(filePath, centerMeshes, loadTextures);
}

void ModelMesh::scaleModelBy(const glm::vec3& scaling) {
	m_assimpModel->downScaleMeshesBy(scaling);
}

void ModelMesh::create() {}

void ModelMesh::draw(const Camera& camera) const {
	m_program->bind();

	m_program->setMat4("MI", m_transformation->makeModelInverseMatrix());
	m_program->setMat4("M", m_transformation->makeModelMatrix());
	m_program->setMat4("V", camera.getView());
	m_program->setMat4("P", camera.getProjection());
	m_program->setVec3("eye", camera.getEye());

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
	m_assimpModel->draw(*m_program);
}

ModelMesh::~ModelMesh() {}