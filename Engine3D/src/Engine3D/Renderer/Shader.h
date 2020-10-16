#pragma once

#include "Engine3D\Core\Config.h"

#include <string>
#include <glm\glm.hpp>

namespace E3D
{
	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetInt(const std::string& name, int data) = 0;
		virtual void SetBool(const std::string& name, bool data) = 0;
		virtual void SetFloat(const std::string& name, float data) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& data) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& data) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& data) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& data) = 0;

		virtual const std::string & GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
	};

	class ShaderLibrary
	{
	public:
		ShaderLibrary(const ShaderLibrary& other) = delete;
		ShaderLibrary& operator=(const ShaderLibrary& other) = delete;

		static void Add(const std::string& name, const Ref<Shader>& shader);
		static void Add(const Ref<Shader>& shader);
		 
		static Ref<Shader> Load(const std::string& name, const std::string& filepath);
		static Ref<Shader> Load(const std::string& filepath);
		 
		static Ref<Shader> Get(const std::string& name);
		 
		static bool Exists(const std::string& name);
	private:
		ShaderLibrary() {};
	private:
		static std::unordered_map<std::string, Ref<Shader>> m_ShaderMap;
	};
}