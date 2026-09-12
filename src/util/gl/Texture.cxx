#include "Texture.hxx"

namespace lgl {
	std::uint32_t tex::TextureFromFile(const std::string& path) {
		std::uint32_t textureID = 0;
		int stbi_width, stbi_height, stbi_channels;
		void* stbi_data = stbi_load(path.c_str(), &stbi_width, &stbi_height, &stbi_channels, 0);
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
			utl::log(utl::LGL_ERROR, "Failed to load texture from file at path: {}", path);
			stbi_image_free(stbi_data);
		}
		return textureID;
	}

	std::uint32_t tex::TextureFromMemory(const std::byte* sourceData, int nBytes) {
		std::uint32_t textureID = 0;
		int stbi_width, stbi_height, stbi_channels;
		void* stbi_data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(sourceData), nBytes, &stbi_width, &stbi_height, &stbi_channels, 0);
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
			std::string errorMessage = "Assimp texture load from memory failed ";
			errorMessage += (nBytes == 0 ? "(zero byte texture data)" : "(non-zero byte texture data, but stbi_load_from_memory failed to decode it)");
			utl::log(utl::LGL_ERROR, "{}", errorMessage);
			stbi_image_free(stbi_data);
		}
		return textureID;
	}

	std::uint32_t tex::TextureFromAssimpTexels(const std::byte* data, std::uint32_t width, std::uint32_t height) {
		std::uint32_t textureID = 0;
		if (height != 0) {
			int nBytes = width * height * 4;
			std::vector<tex::Texel> processedTexels;
			processedTexels.reserve(nBytes);
			for (int i = 0; i < nBytes; i += 4) {
				processedTexels[i / 4].r = data[i + 3]; // BGRA to RGBA data conversion
				processedTexels[i / 4].g = data[i + 2];
				processedTexels[i / 4].b = data[i + 1];
				processedTexels[i / 4].a = data[i + 0];
			}
			const std::byte* processedData = reinterpret_cast<const std::byte*>(processedTexels.data());
			textureID = TextureFromMemory(processedData, nBytes);
		}
		else {
			utl::log(utl::LGL_ERROR, "Assimp texture load from aiTexture failed: zero height indicates compressed texture, which is not supported. Texture data will be ignored.");
		}

		return textureID;
	}
}