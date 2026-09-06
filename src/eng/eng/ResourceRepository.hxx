#pragma once

import lglmodule;

#include <gl/GpuProgram.hxx>
#include <gl/FrameBuffer.hxx>
#include <model/model.hxx>

#include <visual/common/Material.hxx>
#include <visual/mesh/Mesh.hxx>
#include <visual/mesh/ModelMesh.hxx>

namespace lgl {
	class ResourceRepository {
	protected:
		utl::smap<GpuProgram>  m_programs;
		utl::smap<Material>    m_materials;
		utl::smap<Texture>     m_textures;
		utl::smap<Mesh>        m_meshes;
		utl::smap<ModelMesh>   m_modelMeshes;
		utl::smap<FrameBuffer> m_buffers;

	public:
		ResourceRepository() = default;

		ResourceRepository(const ResourceRepository& other) = delete;
		ResourceRepository& operator=(const ResourceRepository& other) = delete;

		ResourceRepository(const ResourceRepository&& other) = delete;
		ResourceRepository& operator=(const ResourceRepository&& other) = delete;

		utl::sptr<GpuProgram> getProgram(utl::cstr name) const { return m_programs.at(name); }
		utl::sptr<Material> getMaterial(utl::cstr name) const { return m_materials.at(name); }
		utl::sptr<Texture> getTexture(utl::cstr name) const { return m_textures.at(name); }
		utl::sptr<Mesh> getMesh(utl::cstr name) const { return m_meshes.at(name); }
		utl::sptr<ModelMesh> getModelMesh(utl::cstr name) const { return m_modelMeshes.at(name); }
		utl::sptr<FrameBuffer> getBuffer(utl::cstr name) const { return m_buffers.at(name); }

		void handleShaderChanges();

	protected:
		virtual void initializeResources() = 0;
	};
}
