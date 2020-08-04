#include "pch.h"
#include "Shader.h"

#include "RenderAPIs\OpenGL\OpenGLShader.h"
#include "Engine3D\Core\Config.h"
#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:		
			{
				E3D_CORE_ASSERT(false, "API::NONE is not supported!");
				return nullptr;
			}
			case RenderAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(filepath);
			}
		}
	}
}