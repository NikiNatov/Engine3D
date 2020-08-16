#include "pch.h"
#include "Buffer.h"

#include "Engine3D\Renderer\RenderAPI.h"

#include "RenderAPIs\OpenGL\OpenGLBuffer.h"

namespace E3D
{
	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size)
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
				return std::make_shared<OpenGLVertexBuffer>(data, size);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count)
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
				return std::make_shared<OpenGLIndexBuffer>(data, count);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}