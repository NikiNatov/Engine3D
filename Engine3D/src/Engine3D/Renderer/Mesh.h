#pragma once

#include "Engine3D\Core\Config.h"
#include "Engine3D\Renderer\Material.h"
#include "Engine3D\Renderer\VertexArray.h"
#

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <optional>

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

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		void ShowMeshWindow();

		inline Ref<Material>& GetMaterial() { return m_Material; }
		inline const Ref<VertexArray>& GetVertexArray() const { return m_VAO; }
	private:
		Ref<Material> m_Material;
		Ref<VertexArray> m_VAO;
	};

	class Node
	{
	public:
		Node(const std::string& name, const std::vector<Ref<Mesh>>& meshes, const glm::mat4& transform);

		void AddChild(const Ref<Node>& child);
		void Draw(const glm::mat4& parentTransform) const;

		void RenderTree(Node*& selectedNode);
	private:
		std::vector<Ref<Node>> m_Children;
		std::vector<Ref<Mesh>> m_Meshes;
		glm::mat4 m_Transform;
		std::string m_Name;
	};

	class Model
	{
	public:
		Model(const std::string& filepath);
		~Model();

		void Draw(const glm::mat4& transform);

		Ref<Mesh> ParseMesh(const aiMesh* mesh);
		Ref<Node> ParseNode(const aiNode* node);

		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const Ref<Node>& GetRootNode() const { return m_Root; }
	private:
		Ref<Node> m_Root;
		std::string m_Filepath;
		std::vector<Ref<Mesh>> m_Meshes;

		Assimp::Importer m_Importer;
		const aiScene* m_Scene;
	};
}