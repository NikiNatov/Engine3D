#pragma once

#include "Engine3D\Renderer\VertexArray.h"
#include "Engine3D\Renderer\Shader.h"

#include <glm\glm.hpp>

namespace E3D
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void ClearScreen() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f)) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}