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
	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}
}