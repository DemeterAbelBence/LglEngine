#pragma once

import lglmodule;
import glmmodule;

#include <visual/mesh/Mesh.hxx>
#include <model/model.hxx>

namespace lgl {
	class ModelMesh : public Mesh {
	private:
		utl::uptr<model::AssimpModel> m_assimpModel;

	public:
		ModelMesh(const utl::str& filePath, bool centerMeshes, bool loadTextures);
		void scaleModelBy(const glm::vec3& scaling);
		void create() override;
		void draw(const Camera& camera) const override;
		~ModelMesh();
	};
}
