#pragma once

#include "Engine3D\Renderer\Shader.h"

#include <unordered_map>

namespace E3D
{
	struct ShaderSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetInt(const std::string& name, int data) override;
		virtual void SetBool(const std::string& name, bool data) override;
		virtual void SetFloat(const std::string& name, float data) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& data) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& data) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& data) override;
		virtual void SetMat4(const std::string& name, int count, const glm::mat4& data) override;

		void UploadUniformInt(const std::string& name, int data);
		void UploadUniformBool(const std::string& name, bool data);
		void UploadUniformFloat(const std::string& name, float data);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& data);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& data);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& data);
		void UploadUniformMat4(const std::string& name, int count, const glm::mat4& data);

		virtual const std::string& GetName() const override { return m_Name; }
	private:
		ShaderSource ReadFromFile(const std::string& filepath);
		void Compile(const ShaderSource& source);
	private:
		uint32_t m_ProgramID;
		std::unordered_map<std::string, int> m_UniformCache;
		std::string m_Name;
	};
}