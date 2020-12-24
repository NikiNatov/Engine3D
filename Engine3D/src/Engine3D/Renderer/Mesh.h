#pragma once

#include "Engine3D\Core\Config.h"
#include "Engine3D\Renderer\Material.h"
#include "Engine3D\Renderer\VertexArray.h"

namespace E3D
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::ivec4 BoneIDs = glm::ivec4(0);
		glm::vec4 BoneWeights = glm::vec4(0.0f);
	};

	class Mesh
	{
	public:
		virtual const Ref<VertexArray>& GetVertexArray() const = 0;

		virtual Ref<Material>& GetMaterial() = 0;
		virtual void SetMaterial(const Ref<Material>& material) = 0;

		virtual const std::string& GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;

		virtual void Render() = 0;

		static Ref<Mesh> CreateStaticMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material);
		static Ref<Mesh> CreateStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);

		static Ref<Mesh> CreateAnimatedMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material);
		static Ref<Mesh> CreateAnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
	};
}