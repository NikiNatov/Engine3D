#include "pch.h"
#include "RenderCommand.h"

#include "RenderAPIs\OpenGL\OpenGLRenderAPI.h"

namespace E3D
{
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();
}