#include "pch.h"
#include "Mesh.h"

#include "Engine3D\Renderer\StaticMesh.h"
#include "Engine3D\Renderer\AnimatedMesh.h"

namespace E3D
{
	Ref<Mesh> Mesh::CreateStaticMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material)
	{
		return CreateRef<StaticMesh>(vertexArray, material);
	}
	Ref<Mesh> Mesh::CreateStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		return CreateRef<StaticMesh>(vertices, indices, material);
	}
	Ref<Mesh> Mesh::CreateAnimatedMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material)
	{
		return CreateRef<AnimatedMesh>(vertexArray, material);
	}
	Ref<Mesh> Mesh::CreateAnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		return CreateRef<AnimatedMesh>(vertices, indices, material);
	}
}
