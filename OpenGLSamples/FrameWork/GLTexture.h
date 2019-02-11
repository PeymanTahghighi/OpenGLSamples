#pragma once

//==========================================================================================
//==========================================================================================
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include<GLFW\glfw3.h>
#include<string>
#include<vector>
//==========================================================================================
//==========================================================================================

//------------------------------------------------------------------------------------------
class TextureManager;
class GLFrameBuffer;
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
class GLTexture
{
public:
	friend class TextureManager;
	friend class GLFrameBuffer;
	//------------------------------------------------------------------------------------------
	enum TEXTURE_TYPE
	{
		UNKOWN = -1,
		DIFFUSE = 0,
		SPECULAR = 1,
		AMBIENT = 2,
		REFLECTION = 3,
		CUBEMAP = 4,
		COLOR_ATTACHMENT = 5,
		DEPTH_ATTACHMENT = 6,
		CUBEMAP_DEPTH_ATTACHMENT = 7,
	};
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	enum TEXTURE_INTERNAL_FORMAT
	{
		NOT_SPECIFIED = 0,
		RGBA8 = 1,
		RGBA16 = 2,
		RGBA16F = 3,
		RGB16F = 4,
		RGB16U = 5,
		RGBA16U = 6,
		RGBA32F = 7,
		RGB32F = 8,
		R32F = 9
	};
	//------------------------------------------------------------------------------------------

public:

	//Constructor and destructors
	GLTexture(const std::string &filename, TEXTURE_TYPE _textureType);
	GLTexture() = delete;
	GLTexture(TEXTURE_TYPE _textureType,TEXTURE_INTERNAL_FORMAT _internalFormat,
		unsigned int width,unsigned int height);
	GLTexture(const std::vector<std::string> &faces);
	GLTexture(const GLTexture &rhs);
	GLTexture &operator = (const GLTexture &rhs);

	~GLTexture();
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	void bind(GLenum textureUnit);

	GLuint getTextureLocation() const { return m_textureLocation; }
	TEXTURE_TYPE getTextureType() const { return m_textureType; }
	GLuint getTextureTarget() const { return m_textureTarget; }
	const std::string& getFileName() const { return m_filename; }
	GLuint getTextureObject() const { return m_textureObject; }
	unsigned int getHeight() const { return m_height; }
	unsigned int getWidth() const { return m_width; }
	//------------------------------------------------------------------------------------------

private:
	//------------------------------------------------------------------------------------------
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObject;
	GLuint m_textureLocation;
	GLint m_internalFormat;
	TEXTURE_TYPE m_textureType;
	unsigned int m_width, m_height;
	//------------------------------------------------------------------------------------------

};
//------------------------------------------------------------------------------------------

