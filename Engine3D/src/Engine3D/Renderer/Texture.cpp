#include "pch.h"
#include "Texture.h"

#include "Engine3D\Renderer\RenderAPI.h"

#include "RenderAPIs\OpenGL\OpenGLTexture.h"

namespace E3D
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const TextureSpecification& spec)
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
			return CreateRef<OpenGLTexture2D>(width, height, spec);
		}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath, const TextureSpecification& spec)
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
				return CreateRef<OpenGLTexture2D>(filepath, spec);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<TextureCubeMap> TextureCubeMap::Create(uint32_t size, const TextureSpecification& spec)
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
				return CreateRef<OpenGLTextureCubeMap>(size, spec);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}