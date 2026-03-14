#include "model.hxx"

#include <cstdlib>

namespace lgl {
	void lgl::model::AssimpMesh::centerToOrigin(utl::vec<Vertex>& vertices) {
		glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
		for (const auto& vertex : vertices) {
			center += vertex.Position;
		}
		utl::uint size = vertices.size();
		center = center / (float)size;

		for (auto& vertex : vertices) {
			vertex.Position -= center;
		}
	}

	lgl::model::AssimpMesh::AssimpMesh(utl::vec<Vertex> vertices, utl::vec<utl::uint> indices,
		utl::vec<Texture> textures, bool centerMeshes) {
		if (centerMeshes) {
			centerToOrigin(vertices);
		}
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}

	void lgl::model::AssimpMesh::downScaleMeshBy(const glm::vec3& scaling) {
		glm::mat4 S = Transformation::makeScaleMatrix(scaling);
		for (auto& vertex : vertices) {
			glm::vec4 newPosition = glm::vec4(vertex.Position, 1.0) * S;
			vertex.Position = glm::vec3(newPosition);
		}
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	}

	void lgl::model::AssimpMesh::draw(const lgl::GpuProgram& program) {
		for (utl::uint i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			utl::str name = textures[i].name;
			program.setInt(name.c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void lgl::model::AssimpMesh::setupMesh() {
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(utl::uint),
			&indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void lgl::model::AssimpModel::loadModel(const utl::str& filePath) {
		asim::Importer modelimport;
		m_scene = (asim::aiScene*)modelimport.ReadFile(filePath, asim::PROCESS_TRIANGULATE | asim::PROCESS_FLIP_UVS);

		if (!m_scene || m_scene->mFlags & asim::SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
			Logger::log(Logger::LGL_ERROR, "Assimp load error: {}\n", modelimport.GetErrorString());
			return;
		}
		m_directory = filePath.substr(0, filePath.find_last_of('/'));

		processNode(m_scene->mRootNode, m_scene);
	}

	void lgl::model::AssimpModel::processNode(aiNode* node, const aiScene* scene) {
		for (utl::uint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene));
		}
		for (utl::uint i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	lgl::model::AssimpMesh lgl::model::AssimpModel::processMesh(asim::aiMesh* mesh, const aiScene* scene) {
		utl::vec<Vertex> vertices;
		utl::vec<utl::uint> indices;
		utl::vec<Texture> textures;

		for (utl::uint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.Position = pos;

			glm::vec3 norm;
			norm.x = mesh->mNormals[i].x;
			norm.y = mesh->mNormals[i].y;
			norm.z = mesh->mNormals[i].z;
			vertex.Normal = glm::normalize(norm);

			if (mesh->mTangents) {
				glm::vec3 tang;
				tang.x = mesh->mTangents[i].x;
				tang.y = mesh->mTangents[i].y;
				tang.z = mesh->mTangents[i].z;
				vertex.Tangent = glm::normalize(tang);

				glm::vec3 bitang = glm::cross(vertex.Normal, vertex.Tangent);
				vertex.BiTangent = glm::normalize(bitang);
			}
			else {
				vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
				vertex.BiTangent = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			if (mesh->mTextureCoords[0]) {
				glm::vec2 tex;
				tex.x = mesh->mTextureCoords[0][i].x;
				tex.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = tex;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		for (utl::uint i = 0; i < mesh->mNumFaces; i++) {
			utl::vec<Vertex*> faceVertices;
			aiFace face = mesh->mFaces[i];
			for (utl::uint j = 0; j < face.mNumIndices; j++) {
				utl::uint index = face.mIndices[j];
				indices.push_back(index);
				faceVertices.push_back(&vertices[index]);
			}

			if (!mesh->mTangents) {
				glm::vec3 pos1 = faceVertices[0]->Position;
				glm::vec3 pos2 = faceVertices[1]->Position;
				glm::vec3 pos3 = faceVertices[2]->Position;

				glm::vec2 uv1 = faceVertices[0]->TexCoords;
				glm::vec2 uv2 = faceVertices[1]->TexCoords;
				glm::vec2 uv3 = faceVertices[2]->TexCoords;

				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				glm::vec3 tang, bitang;
				tang.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tang.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tang.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

				bitang.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
				bitang.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
				bitang.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

				for (auto& v : faceVertices) {
					v->Tangent += tang;
					v->BiTangent += bitang;
					v->Tangent = glm::normalize(v->Tangent);
					v->BiTangent = glm::normalize(v->BiTangent);
				}
			}
		}

		if (m_loadTextures && mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto addTexture = [&](aiTextureType type, utl::str name) {
				utl::vec<Texture> typeTextures = loadMaterialTextures(material, type, name);
				textures.insert(textures.end(), typeTextures.begin(), typeTextures.end());
				};

			addTexture(asim::TEX_TYPE_NORMALS, "texture_normal");
			addTexture(asim::TEX_TYPE_HEIGHT, "texture_height");
			addTexture(asim::TEX_TYPE_DISPLACEMENT, "texture_displacement");

			addTexture(asim::TEX_TYPE_AMBIENT, "texture_ambient");
			addTexture(asim::TEX_TYPE_DIFFUSE, "texture_diffuse");
			addTexture(asim::TEX_TYPE_SPECULAR, "texture_specular");

			addTexture(asim::TEX_TYPE_BASE_COLOR, "texture_base_color");
			addTexture(asim::TEX_TYPE_METALNESS, "texture_metalness");
			addTexture(asim::TEX_TYPE_DIFFUSE_ROUGHNESS, "texture_diffuse_roughness");
			addTexture(asim::TEX_TYPE_AMBIENT_OCCLUSION, "texture_ambient_occlusion");
			addTexture(asim::TEX_TYPE_EMISSION_COLOR, "texture_emissive_color");
			addTexture(asim::TEX_TYPE_GLTF_METALLIC_ROUGHNESS, "texture_gltf_metallic_roughness");
		}

		return AssimpMesh(vertices, indices, textures, m_centerMeshes);
	}

	lgl::utl::vec<lgl::Texture> lgl::model::AssimpModel::loadMaterialTextures(asim::aiMaterial* mat, asim::aiTextureType type, utl::str name) {
		utl::vec<Texture> textures;
		for (utl::uint i = 0; i < mat->GetTextureCount(type); i++) {
			asim::aiString str;
			mat->GetTexture(type, i, &str);
			utl::str aiName = str.C_Str();
			utl::str path = utl::strFormat("{}/{}", m_directory, aiName);
			bool isEmbedded = aiName[0] == '*';

			bool skip = false;
			for (utl::uint i = 0; i < m_texturesLoaded.size(); i++) {
				bool isSamePath = utl::strEquals(m_texturesLoaded[i].path, path);
				bool isSameName = utl::strEquals(m_texturesLoaded[i].name, name);
				if (isSameName && isSamePath) {
					textures.push_back(m_texturesLoaded[i]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				Texture texture;
				texture.name = name;
				texture.path = path;

				if (isEmbedded) {
					int index = std::atoi(&aiName[1]);
					asim::aiTexture* embeddedTexture = m_scene->mTextures[index];
					auto data = reinterpret_cast<utl::byte*>(embeddedTexture->pcData);
					texture.id = tex::TextureFromAssimpTexels(data, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else {
					texture.id = tex::TextureFromFile(path.c_str());
				}

				textures.push_back(texture);
				m_texturesLoaded.push_back(texture);
			}
		}
		return textures;
	}

	lgl::model::AssimpModel::AssimpModel(const utl::str& path, bool centerMeshes, bool loadTextures) {
		m_centerMeshes = centerMeshes;
		m_loadTextures = loadTextures;
		loadModel(path);
	}

	void lgl::model::AssimpModel::downScaleMeshesBy(const glm::vec3& scaling) {
		for (auto& mesh : m_meshes) {
			mesh.downScaleMeshBy(scaling);
		}
	}

	void lgl::model::AssimpModel::draw(const lgl::GpuProgram& program) {
		for (utl::uint i = 0; i < m_meshes.size(); i++) {
			m_meshes[i].draw(program);
		}
	}
}