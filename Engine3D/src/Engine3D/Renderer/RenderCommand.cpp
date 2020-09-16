#include "pch.h"
#include "RenderCommand.h"

#include "RenderAPIs\OpenGL\OpenGLRenderAPI.h"

namespace E3D
{
	Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();
}