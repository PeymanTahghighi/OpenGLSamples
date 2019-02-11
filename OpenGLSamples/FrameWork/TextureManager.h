#pragma once
//==============================================================================================================
//==============================================================================================================
#include<vector>
#include<string>
#include"Singleton.h"
#include"ResourceManager.h"
//==============================================================================================================
//==============================================================================================================

//--------------------------------------------------------------------------------------------------------------
class GLTexture;
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
class TextureManager : public ResourceManager , public Singleton<TextureManager>
{
	struct TextureData
	{
		std::string filePath;
		int width, height, nrChannels;
		unsigned char *data;
		unsigned int textureObject;

		TextureData() : filePath(""),
			width(0),
			height(0),
			nrChannels(0),
			data(nullptr)
		{

		}
	};
public:
	//constructors
	TextureManager();
	TextureManager(const TextureManager &rhs) = delete;
	TextureManager &operator=(const TextureManager &rhs) = delete;
	~TextureManager();
	//--------------------------------------------------------------------------------------------------------------

	//functionality
	void loadTexture(GLTexture &texture, const std::string &filePath);
	void loadCubemapTexture(GLTexture &texture, const std::vector<std::string> &faces);
	void unloadAll() override;
	//--------------------------------------------------------------------------------------------------------------


private:
	bool textureAlreadyLoaded(TextureData &textureData,const std::string &filePath);

private:
	std::vector<TextureData> m_textures;
};
//--------------------------------------------------------------------------------------------------------------

