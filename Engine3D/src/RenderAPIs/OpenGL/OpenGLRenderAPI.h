#pragma once

#include "Engine3D\Renderer\RenderAPI.h"

namespace E3D
{
	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void ClearScreen() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}