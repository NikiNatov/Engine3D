#include "pch.h"
#include "Renderer.h"

namespace E3D
{
	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	void Renderer::BeginScene()
	{
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		RenderCommand::DrawIndexed(vertexArray, shader, transform);
	}
}