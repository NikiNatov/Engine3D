#pragma once

#include "Engine3D\Core\Config.h"

namespace E3D
{
	struct FramebufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;
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

		virtual uint32_t GetColorAttachment() const = 0;
		virtual uint32_t GetDepthAttachment() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}