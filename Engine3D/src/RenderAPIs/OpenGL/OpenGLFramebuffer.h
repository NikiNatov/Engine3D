#pragma once

#include "Engine3D\Renderer\Framebuffer.h"

namespace E3D
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		virtual void Init() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_BufferSpec; }

		virtual const Ref<Texture>& GetColorAttachment() const override { return m_ColorAttachment; }
		virtual const Ref<Texture2D>& GetDepthAttachment() const override { return m_DepthAttachment; }

		virtual void SetTextureTarget(TextureTarget texTarget, uint32_t mipLevel) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	private:
		uint32_t m_FramebufferID = 0;
		Ref<Texture> m_ColorAttachment = 0;
		Ref<Texture2D> m_DepthAttachment = 0;
		FramebufferSpecification m_BufferSpec;
	};
}