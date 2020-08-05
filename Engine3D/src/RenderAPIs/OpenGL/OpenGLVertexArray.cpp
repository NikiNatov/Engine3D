#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad\glad.h>

namespace E3D
{
	GLenum DataTypeToOpenGLType(DataType type)
	{
		switch (type)
		{
			case DataType::Int:		return GL_INT;
			case DataType::Int2:	return GL_INT;
			case DataType::Int3:	return GL_INT;
			case DataType::Int4:	return GL_INT;
			case DataType::Float:	return GL_FLOAT;
			case DataType::Float2:	return GL_FLOAT;
			case DataType::Float3:	return GL_FLOAT;
			case DataType::Float4:	return GL_FLOAT;
			case DataType::Mat4:	return GL_FLOAT;
			case DataType::Bool:	return GL_BOOL;
		}

		E3D_CORE_ASSERT(false, "Unknown Data Type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_VertexArrayID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}
	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_VertexArrayID);
	}
	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		const BufferLayout& layout = vertexBuffer->GetLayout();

		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		for (auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexAttributeIndex);
			glVertexAttribPointer(m_VertexAttributeIndex, element.GetComponentCount(), DataTypeToOpenGLType(element.Type), (element.Normalized ? GL_TRUE : GL_FALSE), layout.GetStride(), (void*)element.Offset);
			m_VertexAttributeIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}