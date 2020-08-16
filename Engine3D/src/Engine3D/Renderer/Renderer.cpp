#include "pch.h"
#include "Renderer.h"

namespace E3D
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
	void Renderer::BeginScene(Camera& camera, const glm::mat4& transform)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
	}
	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		auto& material = mesh->GetMaterial();
		auto& shader = material->GetShader();
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		shader->SetFloat3("u_CameraPosition", glm::vec3(0.0f));
		shader->SetFloat3("u_Light.Direction", glm::vec3(1.0f, -1.0f, -1.0f));
		shader->SetFloat3("u_Light.Ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		shader->SetFloat3("u_Light.Diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat3("u_Light.Specular", glm::vec3(1.0f, 1.0f, 1.0f));

		material->Bind();

		auto& vao = mesh->GetVertexArray();
		vao->Bind();
		RenderCommand::DrawIndexed(vao);
	}
}