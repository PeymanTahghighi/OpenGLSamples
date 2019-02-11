#pragma once
//==========================================================================================
//==========================================================================================
#include<vector>
//==========================================================================================
//==========================================================================================

//------------------------------------------------------------------------------------------
class GLTexture;
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
class GLFrameBuffer
{
public:
	enum FRAMEBUFFER_ATTACHMENT
	{
		COLOR_ATTACHMENT = 0,
		DEPTH_ATTACHMENT = 1,
		CUBEMAP_DEPTH_ATTACHMENT = 2,
		RENDERBUFFER_ATTACHMENT = 3,
	};

public:
	//constructor and destructors
	GLFrameBuffer(unsigned int width,unsigned int height);
	GLFrameBuffer(const GLFrameBuffer &rhs);
	GLFrameBuffer &operator =(const GLFrameBuffer &rhs);

	~GLFrameBuffer();
	//------------------------------------------------------------------------------------------

	//functionality
	void attachToFrameBuffer(FRAMEBUFFER_ATTACHMENT attachmentType,
		GLTexture::TEXTURE_INTERNAL_FORMAT attachmentIntenralFormat = GLTexture::TEXTURE_INTERNAL_FORMAT::NOT_SPECIFIED);
	void beginRender();
	void endRender();
	void enableRenderToAllColorAttachments();
	void disableRenderToAllAttachments();
	void bindAttachedTexture(int,GLenum);

	GLTexture &getAttachedTexture(int i);
	//------------------------------------------------------------------------------------------
private:
	//------------------------------------------------------------------------------------------
	unsigned int m_frameBufferObject;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_renderBuffer;
	unsigned int m_numColorAttachments;
	std::vector<GLTexture> m_attachedTextures;
	//------------------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------------------

