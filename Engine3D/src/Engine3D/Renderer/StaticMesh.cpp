#include "pch.h"
#include "StaticMesh.h"

#include "Engine3D\Renderer\Buffer.h"
#include "Engine3D\Renderer\Renderer.h"

namespace E3D
{
	StaticMesh::StaticMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material)
		: m_VertexArray(vertexArray), m_Material(material)
	{
	}

	StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		BufferLayout layout = {
			{"a_Position",	DataType::Float3, false},
			{"a_TexCoord",	DataType::Float2, false},
			{"a_Normal",	DataType::Float3, false},
			{"a_Tangent",	DataType::Float3, false},
			{"a_Bitangent",	DataType::Float3, false}
		};

		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		vbo->SetLayout(layout);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), indices.size());

		m_VertexArray->AddVertexBuffer(vbo);
		m_VertexArray->SetIndexBuffer(ibo);

		m_Material = material;
	}

	void StaticMesh::Render()
	{
		m_Material->Bind();
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->Unbind();
		m_Material->Unbind();
	}

	
}
