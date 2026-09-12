#pragma once

import utl;
import asim;
import std;

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
			std::vector<Vertex> vertices;
			std::vector<std::uint32_t> indices;
			std::vector<Texture> textures;

		private:
			std::uint32_t m_VAO, m_VBO, m_EBO;
		private:
			void centerToOrigin(std::vector<Vertex>& vertices);

		public:
			AssimpMesh(std::vector<Vertex> vertices, std::vector<std::uint32_t> indices, std::vector<Texture> textures, bool centerMeshes);
			void downScaleMeshBy(const glm::vec3& scaling);
			void draw(const lgl::GpuProgram& program);
			void setupMesh();
		};

		class AssimpModel {
		private:
			std::vector<AssimpMesh> m_meshes;
			std::string m_directory;
			std::vector<Texture> m_texturesLoaded;
			asim::aiScene* m_scene;
			bool m_centerMeshes = false;
			bool m_loadTextures = true;

		private:
			void loadModel(const std::string& filePath);
			void processNode(asim::aiNode* node, const aiScene* scene);
			AssimpMesh processMesh(asim::aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(asim::aiMaterial* mat, asim::aiTextureType type, const std::string& typeName);

		public:
			AssimpModel(const std::string& path, bool centerMeshes, bool loadTextures);
			void downScaleMeshesBy(const glm::vec3& scaling);
			void draw(const GpuProgram& program);
		};
	}
}