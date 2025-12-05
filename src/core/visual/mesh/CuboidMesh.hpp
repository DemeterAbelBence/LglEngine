#pragma once 

#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>

#include <visual/mesh/Mesh.hpp>

namespace lgl {
	class LGL_API CuboidMesh : public Mesh {

	private:
		struct VertexData {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoordinate;
		};
		std::vector<VertexData> m_vertexData;

	private:
		float m_height;
		float m_width;
		float m_length;

		std::array<glm::vec3, 8> m_points;

	private:
		void createPoints();
		void createSide(const std::array<UINT, 4>& indices);
		void setBufferData(const void* data, UINT size);

	public:
		CuboidMesh(glm::vec3 dimensions);
		glm::vec3 calculateCenter() const;
		void setResources(std::shared_ptr<GpuProgram> program,
			std::shared_ptr<Material> material,
			const std::vector<std::shared_ptr<Texture>>& textures);

		void create() override;
		void draw(const Camera& camera) const override;
	};
}