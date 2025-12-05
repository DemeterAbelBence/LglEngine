#pragma once

#include <gl/BaseDefs.hpp>
#include <gl/VertexArray.hpp>
#include <gl/GpuProgram.hpp>
#include <gl/Texture.hpp>

#include <visual/common/Material.hpp>

#include <math/Transformation.hpp>
#include <math/Camera.hpp>

namespace lgl {
	class LGL_API Mesh {

	protected:
		UPTR<VertexArray> m_vertexArray;
		UPTR<VertexBuffer> m_vertexBuffer;
		UPTR<VertexBufferLayout> m_vertexBufferLayout;

		SPTR<GpuProgram> m_program;
		SPTR<Material> m_material;
		SPTR<Transformation> m_transformation;

		TVEC<Texture> m_textures;

		UINT m_primitiveType;
		UINT m_meshFrame;

	public:
		Mesh();

		void setProgram(SPTR<GpuProgram> program) { m_program = program; }
		void setMaterial(SPTR<Material> material) { m_material = material; }
		void setTransformation(SPTR<Transformation> transformation) { m_transformation = transformation; }
		void addTexture(SPTR<Texture> texture) { m_textures.push_back(texture); }

		const SPTR<GpuProgram>& getProgram() const { return m_program; }
		const SPTR<Material>& getMaterial() const { return m_material; }
		const SPTR<Transformation>& getTransformation() const { return m_transformation; }
		const TVEC<Texture>& getTextures() const { return m_textures; }

		void setPrimitive(UINT type) { m_primitiveType = type; }
		UINT getPrimitiveType() const { return m_primitiveType; }

		void setAllUniforms() const;
		void drawMeshFrame(const Camera& camera);

		virtual void create() = 0;
		virtual void draw(const Camera& camera) const = 0;

		~Mesh();
	};
}