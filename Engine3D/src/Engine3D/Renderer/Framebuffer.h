#pragma once

#include "Engine3D\Core\Config.h"

#include "Engine3D\Renderer\Texture.h"

namespace E3D
{
	enum class TextureTarget
	{
		None = 0,
		Texture2D, 
		CubeMapPositiveX, CubeMapNegativeX, CubeMapPositiveY, CubeMapNegativeY, CubeMapPositiveZ, CubeMapNegativeZ
	};

	enum class Attachment
	{
		None = -1,
		Color0, Color1, Color2, Color3, Color4, Color5, Color6, Color7, 
		Depth, Stencil, DepthStencil
	};

	struct FramebufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		TextureTarget TexTarget;
		TextureFormat ColorAttachmentFormat;
		uint32_t Samples = 1;

		FramebufferSpecification()
		{
		}

		FramebufferSpecification(uint32_t width, uint32_t height, TextureTarget target, TextureFormat colorAttachmentFormat, uint32_t samples)
			: Width(width), Height(height), TexTarget(target), ColorAttachmentFormat(colorAttachmentFormat), Samples(samples)
		{
		}
	};
	
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Init() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual const Ref<Texture>& GetColorAttachment() const = 0;
		virtual const Ref<Texture2D>& GetDepthAttachment() const = 0;

		virtual void SetTextureTarget(TextureTarget texTarget, uint32_t mipLevel) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}