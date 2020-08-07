#include "pch.h"
#include "Framebuffer.h"

#include "Engine3D\Renderer\RenderAPI.h"
#include "RenderAPIs\OpenGL\OpenGLFramebuffer.h"

namespace E3D
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:
			{
				E3D_CORE_ASSERT(false, "RendererAPI::API::NONE is not supported!");
				return nullptr;
			}
			case RenderAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLFramebuffer>(spec);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}