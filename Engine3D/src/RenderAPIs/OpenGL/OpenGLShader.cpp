#include "pch.h"
#include "OpenGLShader.h"

#include <glad\glad.h>

#include <glm\gtc\type_ptr.hpp>

namespace E3D
{
	GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		E3D_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		ShaderSource& source = ReadFromFile(filepath);
		Compile(source);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		int count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
	}
	void OpenGLShader::Bind()
	{
		glUseProgram(m_ProgramID);
	}
	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetInt(const std::string& name, int data)
	{
		UploadUniformInt(name, data);
	}
	void OpenGLShader::SetBool(const std::string& name, bool data)
	{
		UploadUniformBool(name, data);
	}
	void OpenGLShader::SetFloat(const std::string& name, float data)
	{
		UploadUniformFloat(name, data);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& data)
	{
		UploadUniformFloat2(name, data);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& data)
	{
		UploadUniformFloat3(name, data);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& data)
	{
		UploadUniformFloat4(name, data);
	}
	void OpenGLShader::SetMat4(const std::string& name, int count, const glm::mat4& data)
	{
		UploadUniformMat4(name, count, data);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform1i(location, data);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());
			m_UniformCache[name] = location;
			glUniform1i(location, data);
		}
	}
	void OpenGLShader::UploadUniformBool(const std::string& name, bool data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform1i(location, data);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());
			m_UniformCache[name] = location;
			glUniform1i(location, data);
		}
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform1f(location, data);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());

			if (location < 0)
			{
				E3D_CORE_LOG_ERROR("Failed to upload uniform {0}", name);
				return;
			}

			m_UniformCache[name] = location;
			glUniform1f(location, data);
		}
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform2f(location, data.x, data.y);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());

			if (location < 0)
			{
				E3D_CORE_LOG_ERROR("Failed to upload uniform {0}", name);
				return;
			}

			m_UniformCache[name] = location;
			glUniform2f(location, data.x, data.y);
		}
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform3f(location, data.x, data.y, data.z);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());

			if (location < 0)
			{
				E3D_CORE_LOG_ERROR("Failed to upload uniform {0}", name);
				return;
			}

			m_UniformCache[name] = location;
			glUniform3f(location, data.x, data.y, data.z);
		}
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniform4f(location, data.x, data.y, data.z, data.w);
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());

			if (location < 0)
			{
				E3D_CORE_LOG_ERROR("Failed to upload uniform {0}", name);
				return;
			}

			m_UniformCache[name] = location;
			glUniform4f(location, data.x, data.y, data.z, data.w);
		}
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, int count, const glm::mat4& data)
	{
		glUseProgram(m_ProgramID);

		if (m_UniformCache.find(name) != m_UniformCache.end())
		{
			int location = m_UniformCache[name];
			glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(data));
		}
		else
		{
			int location = glGetUniformLocation(m_ProgramID, name.c_str());

			if (location < 0)
			{
				E3D_CORE_LOG_ERROR("Failed to upload uniform {0}", name);
				return;
			}

			m_UniformCache[name] = location;
			glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(data));
		}
	}

	ShaderSource OpenGLShader::ReadFromFile(const std::string& filepath)
	{
		std::stringstream ss[2];
		std::ifstream file(filepath, std::ios::in | std::ios::binary);

		enum class ShaderType { None = -1, VertexShader, FragmentShader };
		ShaderType type;

		std::string line;

		if (file.is_open())
		{
			while (getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VertexShader;
					else if(line.find("fragment") != std::string::npos)
						type = ShaderType::FragmentShader;
				}
				else
				{
					ss[(int)type] << line << "\n";
				}
			}
		}
		else
			E3D_CORE_ASSERT(false, "Failed to open shader file: ", filepath);

		return { ss[0].str(), ss[1].str() };
	}

	void OpenGLShader::Compile(const ShaderSource& source)
	{
		const char* vertexSrc = source.VertexSource.c_str();
		const char* fragmentSrc = source.FragmentSource.c_str();

		uint32_t program;

		uint32_t vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vertexSrc, nullptr);
		glCompileShader(vertexShaderID);

		int status;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
		
		if (!status)
		{
			int length;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)alloca(length);

			glGetShaderInfoLog(vertexShaderID, length, &length, message);

			E3D_CORE_LOG_ERROR("Vertex shader compilation failure: {0}", message);
			return;
		}

		uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fragmentSrc, nullptr);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			int length;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)alloca(length);

			glGetShaderInfoLog(fragmentShaderID, length, &length, message);

			E3D_CORE_LOG_ERROR("Fragment shader compilation failure: {0}", message);
			return;
		}

		program = glCreateProgram();
		glAttachShader(program, vertexShaderID);
		glAttachShader(program, fragmentShaderID);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (!status)
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)alloca(length);

			glGetProgramInfoLog(program, length, &length, message);

			E3D_CORE_LOG_ERROR("Program linking failure: {0}", message);
			return;
		}

		m_ProgramID = program;

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

}