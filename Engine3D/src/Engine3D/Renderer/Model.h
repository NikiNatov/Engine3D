#pragma once

#include "Engine3D\Renderer\Mesh.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>

#include "Engine3D\Renderer\Animation\Animation.h"

namespace E3D
{
	struct ModelNode
	{
		std::string Name;
		glm::mat4 Transform;
		std::vector<Ref<Mesh>> Meshes;
		std::vector<Ref<ModelNode>> Children;

		void AddMesh(const Ref<Mesh>& mesh)
		{
			Meshes.push_back(mesh);
		}

		void AddChild(const Ref<ModelNode>& child)
		{
			Children.push_back(child);
		}
	};

	class Model
	{
		friend class Animator;
	public:
		Model();
		Model(const std::string& filepath);
		~Model();

		void Update(Timestep ts);
		void Render();

		void LoadFromFile(const std::string& filepath);
		void AddMesh(const Ref<Mesh>& mesh);

		inline const std::vector<Ref<Mesh>>& GetMeshList() const { return m_Meshes; }
		inline const Ref<Mesh>& GetMesh(uint32_t index) const { return m_Meshes[index]; }
		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const std::string& GetName() const { return m_Name; }
		inline const Ref<ModelNode>& GetRootNode() const { return m_RootNode; }
		inline Bone& RootBone() { return m_RootBone; }
	private:
		Ref<Mesh> ProcessMesh(const aiMesh* mesh);
		Ref<ModelNode> ProcessNode(const aiNode* node);
		Ref<Animation> ProcessAnimation(const aiAnimation* animation);
		bool BuildSkeleton(aiNode* rootNode, Bone& rootBone);
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::string m_Name;
		std::string m_Filepath;

		Ref<ModelNode> m_RootNode;

		Assimp::Importer m_Importer;
		const aiScene* m_Scene;

		std::unordered_map<std::string, std::pair<int, glm::mat4>> m_BoneInfo;

		Bone m_RootBone;
		std::vector<Ref<Animation>> m_Animations;
		Ref<Animator> m_Animator;
	};
}