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

		virtual uint32_t GetColorAttachment() const override { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachment() const override { return m_DepthAttachment; }
	private:
		uint32_t m_FramebufferID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_BufferSpec;
	};
}