//====================================================================================================================
//====================================================================================================================
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include"stb_image.h"
#include "TextureManager.h"
#include"GLTexture.h"
//====================================================================================================================
//====================================================================================================================

//--------------------------------------------------------------------------------------------------------------------
TextureManager::TextureManager()
{
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
TextureManager::~TextureManager()
{
	unloadAll();
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
void TextureManager::loadTexture(GLTexture &_texture, const std::string &_filePath)
{
	TextureData textureData;
	bool loaded = textureAlreadyLoaded(textureData,_filePath);
	if (loaded)
	{
		_texture.m_textureObject = textureData.textureObject;
		_texture.m_width = textureData.width;
		_texture.m_height = textureData.height;
	}
	else
	{
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(_filePath.c_str(), &width, &height, &nrChannels, 0);
		assert(data != nullptr);

		glGenTextures(1, &_texture.m_textureObject);
		glBindTexture(_texture.m_textureTarget, _texture.m_textureObject);

		textureData.data = data;
		textureData.filePath = _filePath;
		textureData.height = height;
		textureData.nrChannels = nrChannels;
		textureData.width = width;
		textureData.textureObject = _texture.m_textureObject;
		_texture.m_width = width;
		_texture.m_height = height;

		m_textures.push_back(textureData);


		glTexImage2D(_texture.m_textureTarget, 0, GL_RGB, textureData.width,
			textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);

		glTexParameteri(_texture.m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(_texture.m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(_texture.m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(_texture.m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(_texture.m_textureTarget, 0);
	}
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
void TextureManager::loadCubemapTexture(GLTexture &_texture, const std::vector<std::string> &_faces)
{
	glGenTextures(1, &_texture.m_textureObject);
	glBindTexture(_texture.m_textureTarget, _texture.m_textureObject);

	
	for (int i = 0; i < 6; ++i)
	{
		TextureData textureData;
		bool loaded = textureAlreadyLoaded(textureData, _faces[i]);
		if (loaded)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 
				textureData.width, textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);
		}
		else
		{
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(_faces[i].c_str(), &width, &height, &nrChannels, 0);
			assert(data != nullptr);
			textureData.data = data;
			textureData.filePath = _faces[i];
			textureData.height = height;
			textureData.width = width;
			textureData.nrChannels = nrChannels;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				textureData.width, textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);

		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
bool TextureManager::textureAlreadyLoaded(TextureData &textureData, const std::string &filePath)
{
	for (auto & member : m_textures)
	{
		if (strcmp(filePath.c_str(), member.filePath.c_str()) == 0)
		{
			textureData = member;
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
void TextureManager::unloadAll()
{
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		delete m_textures[i].data;
		glDeleteTextures(1, &m_textures[i].textureObject);
	}

	m_textures.clear();
}
//--------------------------------------------------------------------------------------------------------------------
