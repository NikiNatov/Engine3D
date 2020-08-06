#include "pch.h"
#include "Texture.h"

#include "Engine3D\Renderer\RenderAPI.h"

#include "RenderAPIs\OpenGL\OpenGLTexture.h"

namespace E3D
{
	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
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
				return std::make_shared<OpenGLTexture2D>(filepath);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}