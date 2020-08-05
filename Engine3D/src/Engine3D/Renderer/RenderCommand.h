#pragma once

#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	class RenderCommand
	{
	public:
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

	private:
		static RenderAPI* s_RenderAPI;
	};
}