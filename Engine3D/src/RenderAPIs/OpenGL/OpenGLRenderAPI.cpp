#include "pch.h"
#include "OpenGLRenderAPI.h"

#include <glad\glad.h>

namespace E3D
{
	void OpenGLRenderAPI::Init()
	{
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRenderAPI::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}