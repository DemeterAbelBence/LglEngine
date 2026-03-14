#pragma once 

import lglmodule;
import glmmodule;

#include <visual/mesh/Mesh.hxx>

namespace lgl {
	class CuboidMesh : public Mesh {

	private:
		struct VertexData {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoordinate;
		};
		utl::vec<VertexData> m_vertexData;

	private:
		float m_height;
		float m_width;
		float m_length;

		utl::arr<glm::vec3, 8> m_points;

	private:
		void createPoints();
		void createSide(const utl::arr<utl::uint, 4>& indices);
		void setBufferData(utl::cmemptr data, utl::uint size);

	public:
		CuboidMesh(glm::vec3 dimensions);
		glm::vec3 calculateCenter() const;
		void setResources(utl::sptr<GpuProgram> program, utl::sptr<Material> material, const utl::vec<utl::sptr<Texture>>& textures);

		void create() override;
		void draw(const Camera& camera) const override;
	};
}