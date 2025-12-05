#pragma once

#include <gl/BaseDefs.hpp>

#include <visual/mesh/Mesh.hpp>

#include <asim/asim.hpp>

namespace lgl {
	class LGL_API ModelMesh : public Mesh {
	private:
		std::unique_ptr<asim::AssimpModel> m_assimpModel;

	public:
		ModelMesh(const std::string& filePath, bool centerMeshes, bool loadTextures);
		void scaleModelBy(const glm::vec3& scaling);
		void create() override;
		void draw(const Camera& camera) const override;
		~ModelMesh();
	};
}
