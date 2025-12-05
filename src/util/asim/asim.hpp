#pragma once

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <GLM/glm.hpp>
#include <STB/stb_image.h> 

#include <gl/BaseDefs.hpp>
#include <gl/Texture.hpp>

#include <math/Camera.hpp>
#include <math/Transformation.hpp>

namespace lgl {
	namespace asim {
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
			std::vector<UINT> indices;
			std::vector<Texture> textures;

		private:
			UINT VAO, VBO, EBO;

		private:
			void centerToOrigin(std::vector<Vertex>& vertices);

		public:
			AssimpMesh(std::vector<Vertex> vertices, std::vector<UINT> indices, std::vector<Texture> textures, bool centerMeshes);
			void downScaleMeshBy(const glm::vec3& scaling);
			void draw(const GpuProgram& program);
			void setupMesh();
		};

		class AssimpModel {
		private:
			std::vector<AssimpMesh> m_meshes;
			std::string m_directory;
			std::vector<Texture> m_texturesLoaded;
			aiScene* m_scene;
			bool m_centerMeshes = false;
			bool m_loadTextures = true;

		private:
			void loadModel(const std::string& filePath);
			void processNode(aiNode* node, const aiScene* scene);
			AssimpMesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		public:
			AssimpModel(const std::string& path, bool centerMeshes, bool loadTextures);
			void downScaleMeshesBy(const glm::vec3& scaling);
			void draw(const GpuProgram& program);
		};
	}
}