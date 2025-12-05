#include "Texture.hpp"

using namespace lgl;

UINT tex::TextureFromFile(CSTRING path) {
	 UINT textureID = 0;
	 int stbi_width, stbi_height, stbi_channels;
	 BYTE* stbi_data = stbi_load(path, &stbi_width, &stbi_height, &stbi_channels, 0);
	 if (stbi_data) {
		 GLenum format = GL_RED;
		 if (stbi_channels == 1) {
			 format = GL_RED;
		 }
		 if (stbi_channels == 3) {
			 format = GL_RGB;
		 }
		 if (stbi_channels == 4) {
			 format = GL_RGBA;
		 }

		 glGenTextures(1, &textureID);
		 glBindTexture(GL_TEXTURE_2D, textureID);
		 glTexImage2D(GL_TEXTURE_2D, 0, format, stbi_width, stbi_height, 0, format, GL_UNSIGNED_BYTE, stbi_data);
		 glGenerateMipmap(GL_TEXTURE_2D);

		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		 stbi_image_free(stbi_data);
	 }
	 else {
		 std::cout << "Texture failed to file load at path: " << path << std::endl;
		 stbi_image_free(stbi_data);
	 }
	 return textureID;
}

UINT tex::TextureFromMemory(BYTE* sourceData, int nBytes) {
	UINT textureID = 0;
	int stbi_width, stbi_height, stbi_channels;
	BYTE* stbi_data = stbi_load_from_memory(sourceData, nBytes, &stbi_width, &stbi_height, &stbi_channels, 0);
	if (stbi_data) {
		GLenum format = GL_RED;
		if (stbi_channels == 1) {
			format = GL_RED;
		}
		if (stbi_channels == 3) {
			format = GL_RGB;
		}
		if (stbi_channels == 4) {
			format = GL_RGBA;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, stbi_width, stbi_height, 0, format, GL_UNSIGNED_BYTE, stbi_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(stbi_data);
	}
	else {
		std::cout << "Assimp texture load from memory failed " << std::endl;
		stbi_image_free(stbi_data);
	}
	return textureID;
}

UINT tex::TextureFromAssimpTexels(BYTE* data, UINT width, UINT height) {
	UINT textureID = 0;
	if (height != 0) {
		int nBytes = width * height * 4;
		std::vector<Texel> processedTexels;
		processedTexels.reserve(nBytes);
		for (int i = 0; i < nBytes; i += 4) {
			processedTexels[i / 4].r = data[i + 3]; // BGRA to RGBA data conversion
			processedTexels[i / 4].g = data[i + 2];
			processedTexels[i / 4].b = data[i + 1];
			processedTexels[i / 4].a = data[i + 0];
		}
		BYTE* processedData = reinterpret_cast<BYTE*>(processedTexels.data());
		textureID = TextureFromMemory(processedData, nBytes);
	}
	else {
		std::cout << "Assimp texture load from aiTexture failed: compressed textures not supported" << std::endl;
	}		

	return textureID;
}