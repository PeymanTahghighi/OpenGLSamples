//====================================================================================================================
//====================================================================================================================
#include<iostream>
#include<cassert>
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include<SOIL\SOIL.h>
#include "GLTexture.h"
#include"TextureManager.h"
//====================================================================================================================
//====================================================================================================================

//--------------------------------------------------------------------------------------------------------------------
GLTexture::GLTexture(const std::string &filename, TEXTURE_TYPE _textureType) :
	m_filename(filename),
	m_textureObject(0),
	m_textureType(_textureType),
	m_textureTarget(GL_TEXTURE_2D),
	m_textureLocation(-1),
	m_width(0),
	m_height(0)
{
	assert((_textureType != TEXTURE_TYPE::COLOR_ATTACHMENT)
		&& (_textureType != TEXTURE_TYPE::DEPTH_ATTACHMENT)
		&& (_textureType != TEXTURE_TYPE::UNKOWN)
		&& (_textureType != TEXTURE_TYPE::CUBEMAP)
	    && (_textureType != TEXTURE_TYPE::CUBEMAP_DEPTH_ATTACHMENT));

	TextureManager::getInstance()->loadTexture(*this, m_filename);
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
GLTexture::GLTexture(GLTexture::TEXTURE_TYPE _textureType,
	TEXTURE_INTERNAL_FORMAT _internalFormat,
	unsigned int width, unsigned int height) :
	m_filename(""),
	m_textureObject(0),
	m_textureType(_textureType),
	m_textureLocation(-1),
	m_width(width),
	m_height(height)
{

	assert((_textureType != TEXTURE_TYPE::DIFFUSE)
		&& (_textureType != TEXTURE_TYPE::SPECULAR)
		&& (_textureType != TEXTURE_TYPE::UNKOWN)
		&& (_textureType != TEXTURE_TYPE::CUBEMAP)
		&& (_textureType != TEXTURE_TYPE::REFLECTION)
		&& (_textureType != TEXTURE_TYPE::AMBIENT));

	if(_internalFormat == TEXTURE_INTERNAL_FORMAT::NOT_SPECIFIED 
		&& (_textureType == TEXTURE_TYPE::COLOR_ATTACHMENT))

	{
		assert(false);
	}


	glGenTextures(1, &m_textureObject);

	if (_textureType == TEXTURE_TYPE::COLOR_ATTACHMENT)
	{
		m_textureTarget = GL_TEXTURE_2D;
		glBindTexture(m_textureTarget, m_textureObject);
		if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGB16F)
		{
			m_internalFormat = GL_RGB16F;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGBA16F)
		{
			m_internalFormat = GL_RGBA16F;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGBA16U)
		{
			m_internalFormat = GL_RGBA;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGB16U)
		{
			m_internalFormat = GL_RGB;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGBA32F)
		{
			m_internalFormat = GL_RGBA32F;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::RGB32F)
		{
			m_internalFormat = GL_RGB32F;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		else if (_internalFormat == TEXTURE_INTERNAL_FORMAT::R32F)
		{
			m_internalFormat = GL_R32F;
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, GL_R32F, GL_FLOAT, nullptr);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (_textureType == TEXTURE_TYPE::DEPTH_ATTACHMENT)
	{
		m_textureTarget = GL_TEXTURE_2D;
		m_internalFormat == GL_DEPTH_COMPONENT;
		glBindTexture(m_textureTarget, m_textureObject);
		glTexImage2D(m_textureTarget, 0, GL_DEPTH_COMPONENT, width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (_textureType == TEXTURE_TYPE::CUBEMAP_DEPTH_ATTACHMENT)
	{
		m_textureTarget = GL_TEXTURE_CUBE_MAP;
		glBindTexture(m_textureTarget, m_textureObject);
		for (int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	/*else if (_textureType == TEXTURE_TYPE::COMPUTE_SHADER_FLOATING)
	{
		m_textureTarget = GL_TEXTURE_2D;
		glBindTexture(m_textureTarget, m_textureObject);
		glTexStorage2D(m_textureTarget,1, GL_RGBA16F, width, height);
		GLfloat bordercolor[] = { 0.0f,0.0f,0.0f,1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}*/

	
	glBindTexture(m_textureTarget, 0);
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
GLTexture::GLTexture(const std::vector<std::string> &faces) :
	m_filename(""),
	m_textureObject(0),
	m_textureTarget(GL_TEXTURE_CUBE_MAP),
	m_textureLocation(-1),
	m_textureType(TEXTURE_TYPE::CUBEMAP),
	m_width(0),
	m_height(0)
{
	assert(faces.size() == 6);
	TextureManager::getInstance()->loadCubemapTexture(*this, faces);
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
GLTexture::GLTexture(const GLTexture &rhs)
{
	m_filename = rhs.m_filename;
	m_textureLocation = rhs.m_textureLocation;
	m_textureObject = rhs.m_textureObject;
	m_textureType = rhs.m_textureType;
	m_textureTarget = rhs.m_textureTarget;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
GLTexture &GLTexture::operator=(const GLTexture &rhs)
{
	m_filename = rhs.m_filename;
	m_textureLocation = rhs.m_textureLocation;
	m_textureObject = rhs.m_textureObject;
	m_textureType = rhs.m_textureType;
	m_textureTarget = rhs.m_textureTarget;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
GLTexture::~GLTexture()
{
	//we do not do any cleanup here.
	//all cleanups for texture objects goes in
	//TextureManager::unloadAll.
}
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
void GLTexture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_textureObject);
}
//-------------------------------------------------------------------------------------
