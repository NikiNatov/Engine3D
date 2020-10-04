#include "pch.h"
#include "Shader.h"

#include "RenderAPIs\OpenGL\OpenGLShader.h"
#include "Engine3D\Core\Config.h"
#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::m_ShaderMap;

	Ref<Shader> Shader::Create(const std::string& filepath)
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
				return std::make_shared<OpenGLShader>(filepath);
			}
		}

		E3D_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		E3D_CORE_ASSERT(!Exists(name), "Shader with that name already exists!");
		m_ShaderMap[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		E3D_CORE_ASSERT(Exists(name), "Shader does not exist!");
		return m_ShaderMap[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_ShaderMap.find(name) != m_ShaderMap.end();
	}
}