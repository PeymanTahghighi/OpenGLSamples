//==========================================================================================
//==========================================================================================
#include<cassert>
#include"GLTexture.h"
#include "GLFrameBuffer.h"
//==========================================================================================
//==========================================================================================


//------------------------------------------------------------------------------------------
GLFrameBuffer::GLFrameBuffer(unsigned int width, unsigned int height) :
	m_width(width),
	m_height(height),
	m_frameBufferObject(0),
	m_renderBuffer(0),
	m_numColorAttachments(0)
{
	glGenFramebuffers(1, &m_frameBufferObject);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
GLFrameBuffer::~GLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_frameBufferObject);
	glDeleteRenderbuffers(1, &m_renderBuffer);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
GLFrameBuffer::GLFrameBuffer(const GLFrameBuffer &rhs)
{
	m_frameBufferObject = rhs.m_frameBufferObject;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_renderBuffer = rhs.m_renderBuffer;

	for (unsigned int i = 0; i < rhs.m_attachedTextures.size(); ++i)
	{
		m_attachedTextures.push_back(rhs.m_attachedTextures[i]);
	}
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
GLFrameBuffer& GLFrameBuffer::operator=(const GLFrameBuffer &rhs)
{
	m_frameBufferObject = rhs.m_frameBufferObject;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_renderBuffer = rhs.m_renderBuffer;

	for (unsigned int i = 0; i < rhs.m_attachedTextures.size(); ++i)
	{
		m_attachedTextures.push_back(rhs.m_attachedTextures[i]);
	}
	return *this;
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT attachmentType,
	GLTexture::TEXTURE_INTERNAL_FORMAT attachmentIntenralFormat)
{
	if (attachmentType == GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT)
	{
		GLTexture texture(GLTexture::TEXTURE_TYPE::COLOR_ATTACHMENT,attachmentIntenralFormat, m_width, m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments,
			GL_TEXTURE_2D, texture.m_textureObject, 0);
		m_attachedTextures.push_back(texture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_numColorAttachments++;
	}
	else if (attachmentType == GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::DEPTH_ATTACHMENT)
	{
		GLTexture texture(GLTexture::TEXTURE_TYPE::DEPTH_ATTACHMENT,
			GLTexture::TEXTURE_INTERNAL_FORMAT::NOT_SPECIFIED, m_width, m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
			texture.m_textureObject, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_attachedTextures.push_back(texture);
	}
	else if (attachmentType == GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::CUBEMAP_DEPTH_ATTACHMENT)
	{
		GLTexture texture(GLTexture::TEXTURE_TYPE::CUBEMAP_DEPTH_ATTACHMENT,
			GLTexture::TEXTURE_INTERNAL_FORMAT::NOT_SPECIFIED,
			m_width, m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.m_textureObject, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_attachedTextures.push_back(texture);
	}
	else if (attachmentType == GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::RENDERBUFFER_ATTACHMENT)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::endRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::enableRenderToAllColorAttachments()
{
	std::vector<GLenum> attachments;
	/*for (int i = 0; i < m_attachedTextures.size(); ++i)
	{
		if(m_attachedTextures[i].getTextureType() == GLTexture::TEXTURE_TYPE::COLOR_ATTACHMENT)
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}*/
	attachments.push_back(GL_COLOR_ATTACHMENT0 + 0);
	attachments.push_back(GL_COLOR_ATTACHMENT0 + 1);
	attachments.push_back(GL_COLOR_ATTACHMENT0 + 2);

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	glDrawBuffers(attachments.size(), attachments.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::disableRenderToAllAttachments()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
GLTexture & GLFrameBuffer::getAttachedTexture(int i)
{
	assert(i < m_attachedTextures.size());
	return m_attachedTextures[i];
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void GLFrameBuffer::bindAttachedTexture(int attachedTexture, GLenum texture)
{
	assert(attachedTexture < m_attachedTextures.size());
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, m_attachedTextures[attachedTexture].getTextureObject());
}
//------------------------------------------------------------------------------------------
