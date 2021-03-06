#include "pch.h"
#include "Renderer.h"

#include "Engine3D\ResourceManager\TextureManager.h"

#include <glad\glad.h>

namespace E3D
{
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();
	RenderMode Renderer::s_RenderMode = RenderMode::Triangles;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
	void Renderer::BeginScene(Camera& camera, const glm::mat4& transform, const Ref<Skybox>& skybox)
	{
		m_SceneData->ViewMatrix = glm::inverse(transform);
		m_SceneData->ProjectionMatrix = camera.GetProjection();
		m_SceneData->ViewProjectionMatrix = m_SceneData->ProjectionMatrix * m_SceneData->ViewMatrix;
		m_SceneData->Skybox = skybox;
	}
	void Renderer::BeginScene(EditorCamera& camera, const Ref<Skybox>& skybox)
	{
		m_SceneData->ProjectionMatrix = camera.GetProjection();
		m_SceneData->ViewMatrix = camera.GetViewMatrix();
		m_SceneData->ViewProjectionMatrix = m_SceneData->ProjectionMatrix * m_SceneData->ViewMatrix;
		m_SceneData->Skybox = skybox;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{

		auto& material = mesh->GetMaterial();
		auto& shader = material->GetShader();
		shader->Bind();
		shader->SetMat4("u_ViewProjection", 1, m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", 1, transform);
		shader->SetFloat3("u_CameraPosition", glm::inverse(m_SceneData->ViewMatrix)[3]);
		shader->SetFloat3("u_Light.Direction", glm::vec3(0.0f, 0.0f, 0.0));
		shader->SetFloat3("u_Light.Radiance", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetInt("u_IrradianceMap", 4);
		shader->SetInt("u_PrefilterMap", 5);
		shader->SetInt("u_BRDFLUT", 6);
		shader->SetFloat("u_Exposure", m_SceneData->Skybox->GetExposure());


		material->Bind();

		m_SceneData->Skybox->GetIrradianceMap()->Bind(4);
		m_SceneData->Skybox->GetPrefilterMap()->Bind(5);
		TextureManager::GetTexture("PreintegratedFG.bmp")->Bind(6);

		auto& vao = mesh->GetVertexArray();
		vao->Bind();

		if (s_RenderMode == RenderMode::Triangles)
			RenderCommand::DrawIndexed(vao);
		else if (s_RenderMode == RenderMode::Lines)
			RenderCommand::DrawIndexedLines(vao);

		material->Unbind();
		
	}

	void Renderer::Submit(const Ref<Skybox>& skybox, const glm::mat4& transform)
	{
		auto& shader = skybox->GetShader();
		shader->Bind();
		shader->SetMat4("u_View", 1, m_SceneData->ViewMatrix);
		shader->SetMat4("u_Projection", 1, m_SceneData->ProjectionMatrix);
		shader->SetFloat("u_Exposure", skybox->GetExposure());
		shader->SetFloat("u_LOD", skybox->GetLOD());

		m_SceneData->Skybox->GetCubeMap()->Bind(0);

		auto& vao = skybox->GetVertexArray();
		vao->Bind();
		RenderCommand::DrawIndexed(vao);

	}

}