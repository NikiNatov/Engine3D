#pragma once

#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void ClearScreen() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f)) override;
	};
}