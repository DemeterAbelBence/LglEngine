module;

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

export module asimmodule;

export namespace asim {
	// Importer class
	using Importer = Assimp::Importer;

	// Scene types
	using aiScene = ::aiScene;
	using aiNode = ::aiNode;
	using aiMesh = ::aiMesh;
	using aiMaterial = ::aiMaterial;
	using aiTexture = ::aiTexture;
	using aiFace = ::aiFace;
	using aiString = ::aiString;

	// Texture types
	using aiTextureType = ::aiTextureType;

	// Scene flags
	constexpr auto SCENE_FLAGS_INCOMPLETE = AI_SCENE_FLAGS_INCOMPLETE;

	// Texture type constants
	constexpr auto TEX_TYPE_NORMALS = ::aiTextureType_NORMALS;
	constexpr auto TEX_TYPE_HEIGHT = ::aiTextureType_HEIGHT;
	constexpr auto TEX_TYPE_DISPLACEMENT = ::aiTextureType_DISPLACEMENT;
	constexpr auto TEX_TYPE_AMBIENT = ::aiTextureType_AMBIENT;
	constexpr auto TEX_TYPE_DIFFUSE = ::aiTextureType_DIFFUSE;
	constexpr auto TEX_TYPE_SPECULAR = ::aiTextureType_SPECULAR;
	constexpr auto TEX_TYPE_BASE_COLOR = ::aiTextureType_BASE_COLOR;
	constexpr auto TEX_TYPE_METALNESS = ::aiTextureType_METALNESS;
	constexpr auto TEX_TYPE_DIFFUSE_ROUGHNESS = ::aiTextureType_DIFFUSE_ROUGHNESS;
	constexpr auto TEX_TYPE_AMBIENT_OCCLUSION = ::aiTextureType_AMBIENT_OCCLUSION;
	constexpr auto TEX_TYPE_EMISSION_COLOR = ::aiTextureType_EMISSION_COLOR;
	constexpr auto TEX_TYPE_GLTF_METALLIC_ROUGHNESS = ::aiTextureType_GLTF_METALLIC_ROUGHNESS;

	// Post-processing flags
	constexpr auto PROCESS_TRIANGULATE = ::aiProcess_Triangulate;
	constexpr auto PROCESS_FLIP_UVS = ::aiProcess_FlipUVs;
}