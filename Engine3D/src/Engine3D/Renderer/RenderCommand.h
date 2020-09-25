#pragma once

#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RenderAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		inline static void ClearScreen()
		{
			s_RenderAPI->ClearScreen();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count)
		{
			s_RenderAPI->DrawArrays(vertexArray, count);
		}

		inline static void DrawIndexedLines(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexedLines(vertexArray);
		}

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};
}