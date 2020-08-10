#pragma once

#include "Engine3D\Renderer\RenderCommand.h"
#include "Engine3D\Renderer\PerspectiveCamera.h"
#include "Engine3D\Renderer\Camera.h"

namespace E3D
{
	class Renderer
	{
	public:
		static void Init();
		static void WindowResize(uint32_t width, uint32_t height);
		static void BeginScene(Camera& camera, const glm::mat4& transform);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 m_ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}