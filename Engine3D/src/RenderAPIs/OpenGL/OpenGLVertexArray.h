#pragma once

#include "Engine3D\Renderer\VertexArray.h"

namespace E3D
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_VertexArrayID;
		uint32_t m_VertexAttributeIndex = 0;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}