#include "pch.h"
#include "AnimatedMesh.h"
#include "Engine3D\Renderer\RenderCommand.h"


namespace E3D
{
	AnimatedMesh::AnimatedMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material)
		: m_Material(material), m_VertexArray(vertexArray)
	{
	}
	AnimatedMesh::AnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
		: m_Material(material)
	{
		BufferLayout layout = {
			{"a_Position",		DataType::Float3, false},
			{"a_TexCoord",		DataType::Float2, false},
			{"a_Normal",		DataType::Float3, false},
			{"a_Tangent",		DataType::Float3, false},
			{"a_Bitangent",		DataType::Float3, false},
			{"a_BoneIDs",		DataType::Int4,	  false},
			{"a_BoneWeights",	DataType::Float4, false}
		};

		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(AnimatedVertex));
		vbo->SetLayout(layout);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), indices.size());

		m_VertexArray->AddVertexBuffer(vbo);
		m_VertexArray->SetIndexBuffer(ibo);
	}

	void AnimatedMesh::Render()
	{
		m_Material->Bind();
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->Unbind();
		m_Material->Unbind();
	}
}