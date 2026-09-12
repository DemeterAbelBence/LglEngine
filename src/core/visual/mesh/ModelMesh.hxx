#pragma once

import utl;
import glm;

#include <visual/mesh/Mesh.hxx>
#include <model/model.hxx>

namespace lgl {
	class ModelMesh : public Mesh {
	private:
		std::unique_ptr<model::AssimpModel> m_assimpModel;

	public:
		ModelMesh(const std::string& filePath, bool centerMeshes, bool loadTextures);
		void scaleModelBy(const glm::vec3& scaling);
		void create() override;
		void draw(const Camera& camera) const override;
		~ModelMesh();
	};
}
