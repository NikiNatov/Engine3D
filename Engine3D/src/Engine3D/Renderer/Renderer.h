#pragma once

#include "Engine3D\Renderer\RenderCommand.h"

namespace E3D
{
	class Renderer
	{
	public:
		static void Init();
		static void BeginScene();
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); }
	};
}