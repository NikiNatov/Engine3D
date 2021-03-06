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
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void ClearScreen() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		virtual void DrawIndexedLines(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

		static Scope<RenderAPI> Create();
	private:
		static API s_API;
	};
}