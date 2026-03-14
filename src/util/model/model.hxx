#pragma once

import lglmodule;
import asimmodule;

#include <GLEW/glew.h>

#include <gl/Texture.hxx>
#include <gl/GpuProgram.hxx>
#include <math/Camera.hxx>
#include <math/Transformation.hxx>

namespace lgl {
	namespace model {
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 BiTangent;
			glm::vec2 TexCoords;
		};

		class AssimpMesh {
		public:
			utl::vec<Vertex> vertices;
			utl::vec<utl::uint> indices;
			utl::vec<Texture> textures;

		private:
			utl::uint m_VAO, m_VBO, m_EBO;
		private:
			void centerToOrigin(utl::vec<Vertex>& vertices);

		public:
			AssimpMesh(utl::vec<Vertex> vertices, utl::vec<utl::uint> indices, utl::vec<Texture> textures, bool centerMeshes);
			void downScaleMeshBy(const glm::vec3& scaling);
			void draw(const lgl::GpuProgram& program);
			void setupMesh();
		};

		class AssimpModel {
		private:
			utl::vec<AssimpMesh> m_meshes;
			utl::str m_directory;
			utl::vec<Texture> m_texturesLoaded;
			asim::aiScene* m_scene;
			bool m_centerMeshes = false;
			bool m_loadTextures = true;

		private:
			void loadModel(const utl::str& filePath);
			void processNode(asim::aiNode* node, const aiScene* scene);
			AssimpMesh processMesh(asim::aiMesh* mesh, const aiScene* scene);
			utl::vec<Texture> loadMaterialTextures(asim::aiMaterial* mat, asim::aiTextureType type, utl::str typeName);

		public:
			AssimpModel(const utl::str& path, bool centerMeshes, bool loadTextures);
			void downScaleMeshesBy(const glm::vec3& scaling);
			void draw(const GpuProgram& program);
		};
	}
}