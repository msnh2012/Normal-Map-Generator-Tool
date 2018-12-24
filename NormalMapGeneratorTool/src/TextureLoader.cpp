#include "TextureLoader.h"
#include "stb_image.h"
#include "TextureData.h"
#include <GL\glew.h>
#include <iostream>

void TextureManager::getRawImageDataFromFile(const std::string & path, std::vector<unsigned char>& data, int &width, int &height, bool flipImage)
{
	int nrComponents;
	stbi_set_flip_vertically_on_load(flipImage);
	unsigned char* ldata = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	const unsigned int size = width * height * nrComponents;
	for (unsigned int i = 0; i < size; i++)
		data.push_back(ldata[i]);
}

void TextureManager::getTextureDataFromFile(const std::string & path, TextureData & textureData)
{
	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
		textureData.setTextureData(data, width, height, nrComponents);
}

glm::vec2 TextureManager::getImageDimensions(const std::string & path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	free(data);
	return glm::vec2(width, height);
}

unsigned int TextureManager::loadTextureFromFile(const std::string & path, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat = GL_SRGB;
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
		{
			format = GL_RGB;
			internalFormat = GL_SRGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
			internalFormat = GL_SRGB_ALPHA;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
#if _DEBUG
	else
		std::cout << "\nTexture failed to load at path: " << path.c_str() << std::endl;
#endif
	stbi_image_free(data);
	return textureID;
	}

unsigned int TextureManager::loadCubemapFromFile(const std::vector<std::string>& paths, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << paths[i].c_str() << std::endl;
			stbi_image_free(data);
		}
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}

unsigned int TextureManager::loadTextureFromData(TextureData & textureData, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	unsigned char* data = textureData.getTextureData();
	if (data)
	{
		GLenum format = GL_RED;
		if (textureData.getComponentCount() == 1)
			format = GL_RED;
		else if (textureData.getComponentCount() == 3)
			format = GL_RGB;
		else if (textureData.getComponentCount() == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, textureData.getWidth(), textureData.getHeight(), 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
#if _DEBUG
	else
		std::cout << "\nTexture failed to load with texture data " << std::endl;
#endif
	return textureID;
	}

GLenum TextureManager::getTextureFormatFromData(TextureData & textureData)
{
	GLenum format = GL_RED;
	if (textureData.getComponentCount() == 1)
		format = GL_RED;
	else if (textureData.getComponentCount() == 3)
		format = GL_RGB;
	else if (textureData.getComponentCount() == 4)
		format = GL_RGBA;
	return format;
}

GLenum TextureManager::getTextureFormatFromData(int componentCount)
{
	GLenum format = GL_RED;
	if (componentCount == 1)
		format = GL_RED;
	else if (componentCount == 3)
		format = GL_RGB;
	else if (componentCount == 4)
		format = GL_RGBA;
	return format;
}