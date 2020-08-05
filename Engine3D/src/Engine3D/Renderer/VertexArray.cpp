#include "pch.h"
#include "VertexArray.h"

#include "Engine3D\Renderer\RenderAPI.h"
#include "RenderAPIs\OpenGL\OpenGLVertexArray.h"

namespace E3D
{
	Ref<VertexArray> VertexArray::Create()
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
				return std::make_shared<OpenGLVertexArray>();
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}