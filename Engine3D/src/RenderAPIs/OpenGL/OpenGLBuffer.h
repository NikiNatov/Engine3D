#pragma once

#include "Engine3D\Renderer\Buffer.h"

namespace E3D
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* data, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_BufferID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* data, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_BufferID;
		uint32_t m_Count;
	};
}