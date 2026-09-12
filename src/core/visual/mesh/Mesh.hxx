#pragma once

import utl;
import glm;

#include <gl/VertexArray.hxx>
#include <gl/GpuProgram.hxx>
#include <gl/Texture.hxx>

#include <visual/common/Material.hxx>

#include <math/Transformation.hxx>
#include <math/Camera.hxx>

namespace lgl {
	class Mesh {

	protected:
		std::unique_ptr<VertexArray> m_vertexArray;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<VertexBufferLayout> m_vertexBufferLayout;

		std::shared_ptr<GpuProgram> m_program;
		std::shared_ptr<Material> m_material;
		std::shared_ptr<Transformation> m_transformation;

		std::vector<std::shared_ptr<Texture>> m_textures;

		std::uint32_t m_primitiveType;
		std::uint32_t m_meshFrame;

	public:
		Mesh();

		void setProgram(std::shared_ptr<GpuProgram> program) { m_program = program; }
		void setMaterial(std::shared_ptr<Material> material) { m_material = material; }
		void setTransformation(std::shared_ptr<Transformation> transformation) { m_transformation = transformation; }
		void addTexture(std::shared_ptr<Texture> texture) { m_textures.push_back(texture); }

		const std::shared_ptr<GpuProgram>& getProgram() const { return m_program; }
		const std::shared_ptr<Material>& getMaterial() const { return m_material; }
		const std::shared_ptr<Transformation>& getTransformation() const { return m_transformation; }
		const std::vector<std::shared_ptr<Texture>>& getTextures() const { return m_textures; }

		void setPrimitive(std::uint32_t type) { m_primitiveType = type; }
		std::uint32_t getPrimitiveType() const { return m_primitiveType; }

		void setAllUniforms() const;
		void drawMeshFrame(const Camera& camera);

		virtual void create() = 0;
		virtual void draw(const Camera& camera) const = 0;

		~Mesh();
	};
}