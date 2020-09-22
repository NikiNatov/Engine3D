#pragma once

#include "Engine3D\Renderer\RenderCommand.h"
#include "Engine3D\Renderer\EditorCamera.h"
#include "Engine3D\Renderer\Camera.h"

#include "Engine3D\Renderer\Material.h"
#include "Engine3D\Renderer\Mesh.h"
#include "Engine3D\Renderer\SkyBox.h"

namespace E3D
{
	class Renderer
	{
	public:
		static void Init();
		static void WindowResize(uint32_t width, uint32_t height);
		static void BeginScene(Camera& camera, const glm::mat4& transform, const Ref<Skybox>& skybox);
		static void BeginScene(EditorCamera& camera, const Ref<Skybox>& skybox);
		static void EndScene();
		static void Submit(const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Skybox>& skybox, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewProjectionMatrix;
			Ref<Skybox> Skybox;
		};

		static Scope<SceneData> m_SceneData;
	};
}