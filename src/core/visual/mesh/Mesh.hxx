#pragma once

import lglmodule;
import glmmodule;

#include <gl/VertexArray.hxx>
#include <gl/GpuProgram.hxx>
#include <gl/Texture.hxx>

#include <visual/common/Material.hxx>

#include <math/Transformation.hxx>
#include <math/Camera.hxx>

namespace lgl {
	class Mesh {

	protected:
		utl::uptr<VertexArray> m_vertexArray;
		utl::uptr<VertexBuffer> m_vertexBuffer;
		utl::uptr<VertexBufferLayout> m_vertexBufferLayout;

		utl::sptr<GpuProgram> m_program;
		utl::sptr<Material> m_material;
		utl::sptr<Transformation> m_transformation;

		utl::svec<Texture> m_textures;

		utl::uint m_primitiveType;
		utl::uint m_meshFrame;

	public:
		Mesh();

		void setProgram(utl::sptr<GpuProgram> program) { m_program = program; }
		void setMaterial(utl::sptr<Material> material) { m_material = material; }
		void setTransformation(utl::sptr<Transformation> transformation) { m_transformation = transformation; }
		void addTexture(utl::sptr<Texture> texture) { m_textures.push_back(texture); }

		const utl::sptr<GpuProgram>& getProgram() const { return m_program; }
		const utl::sptr<Material>& getMaterial() const { return m_material; }
		const utl::sptr<Transformation>& getTransformation() const { return m_transformation; }
		const utl::svec<Texture>& getTextures() const { return m_textures; }

		void setPrimitive(utl::uint type) { m_primitiveType = type; }
		utl::uint getPrimitiveType() const { return m_primitiveType; }

		void setAllUniforms() const;
		void drawMeshFrame(const Camera& camera);

		virtual void create() = 0;
		virtual void draw(const Camera& camera) const = 0;

		~Mesh();
	};
}