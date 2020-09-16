#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad\glad.h>

namespace E3D
{
	GLenum TextureTargetToOpenGLTarget(TextureTarget target)
	{
		switch (target)
		{
		case TextureTarget::Texture2D:				return GL_TEXTURE_2D;
		case TextureTarget::CubeMapPositiveX:		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case TextureTarget::CubeMapNegativeX:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case TextureTarget::CubeMapPositiveY:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case TextureTarget::CubeMapNegativeY:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case TextureTarget::CubeMapPositiveZ:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case TextureTarget::CubeMapNegativeZ:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		}

		E3D_CORE_ASSERT(false, "Invalid Framebuffer Target!");
		return 0;
	}

	GLenum FramebufferAttachmentToOpenGLAttachment(Attachment attachment)
	{
		switch (attachment)
		{
			case Attachment::Color0:			return GL_COLOR_ATTACHMENT0;
			case Attachment::Color1:			return GL_COLOR_ATTACHMENT1;
			case Attachment::Color2:			return GL_COLOR_ATTACHMENT2;
			case Attachment::Color3:			return GL_COLOR_ATTACHMENT3;
			case Attachment::Color4:			return GL_COLOR_ATTACHMENT4;
			case Attachment::Color5:			return GL_COLOR_ATTACHMENT5;
			case Attachment::Color6:			return GL_COLOR_ATTACHMENT6;
			case Attachment::Color7:			return GL_COLOR_ATTACHMENT7;
			case Attachment::Depth:				return GL_DEPTH_ATTACHMENT;
			case Attachment::DepthStencil:		return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		E3D_CORE_ASSERT(false, "Invalid Framebuffer Attachment!");
		return 0;
	}
	
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_BufferSpec(spec)
	{
		Init();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLFramebuffer::Init()
	{
		if (m_FramebufferID)
			glDeleteFramebuffers(1, &m_FramebufferID);

		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		E3D_CORE_ASSERT((m_BufferSpec.TexTarget != TextureTarget::None), "Texture target not supported by the framebuffer!");

		if (m_BufferSpec.TexTarget == TextureTarget::Texture2D)
			m_ColorAttachment = Texture2D::Create(m_BufferSpec.Width, m_BufferSpec.Height, { m_BufferSpec.ColorAttachmentFormat, TextureFilter::Linear, TextureWrap::Clamp_To_Edge });
		else
			m_ColorAttachment = TextureCubeMap::Create(m_BufferSpec.Width, { m_BufferSpec.ColorAttachmentFormat, TextureFilter::Linear, TextureWrap::Clamp_To_Edge });

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureTargetToOpenGLTarget(m_BufferSpec.TexTarget), m_ColorAttachment->GetTextureID(), 0);
		
		m_DepthAttachment = Texture2D::Create(m_BufferSpec.Width, m_BufferSpec.Height, { TextureFormat::Depth_Stencil, TextureFilter::Linear, TextureWrap::Clamp_To_Edge });

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->GetTextureID(), 0);

		E3D_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer Incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		SetViewport(0, 0, m_BufferSpec.Width, m_BufferSpec.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_BufferSpec.Width = width;
		m_BufferSpec.Height = height;

		Init();
	}

	void OpenGLFramebuffer::SetTextureTarget(TextureTarget texTarget, uint32_t mipLevel)
	{
		if (m_BufferSpec.TexTarget == TextureTarget::Texture2D && m_BufferSpec.TexTarget != texTarget)
			m_ColorAttachment = TextureCubeMap::Create(m_BufferSpec.Width, { m_BufferSpec.ColorAttachmentFormat, TextureFilter::Linear, TextureWrap::Clamp_To_Edge });
		else if(m_BufferSpec.TexTarget != TextureTarget::Texture2D && texTarget == TextureTarget::Texture2D)
			m_ColorAttachment = Texture2D::Create(m_BufferSpec.Width, m_BufferSpec.Height, { m_BufferSpec.ColorAttachmentFormat, TextureFilter::Linear, TextureWrap::Clamp_To_Edge });

		m_BufferSpec.TexTarget = texTarget;

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureTargetToOpenGLTarget(m_BufferSpec.TexTarget), m_ColorAttachment->GetTextureID(), mipLevel);
	}

	void OpenGLFramebuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glViewport(x, y, width, height);
	}

	
}