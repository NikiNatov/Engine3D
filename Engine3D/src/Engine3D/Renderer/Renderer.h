#pragma once

#include "Engine3D\Renderer\RenderCommand.h"

namespace E3D
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray);

		inline static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); }
	};
}