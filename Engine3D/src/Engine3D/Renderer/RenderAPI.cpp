#include "pch.h"
#include "RenderAPI.h"

#include "RenderAPIs\OpenGL\OpenGLRenderAPI.h"

namespace E3D
{
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::Create()
	{
		switch (s_API)
		{
			case RenderAPI::API::None:    E3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
		}

		E3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}