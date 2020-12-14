#include "pch.h"
#include "Model.h"

#include "Engine3D\ResourceManager\MaterialManager.h"
#include "Engine3D\ResourceManager\TextureManager.h"

#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

namespace E3D
{
	static glm::mat4 aiToGLM(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;

		result[0].x = matrix.a1; result[0].y = matrix.b1; result[0].z = matrix.c1; result[0].w = matrix.d1;
		result[1].x = matrix.a2; result[1].y = matrix.b2; result[1].z = matrix.c2; result[1].w = matrix.d2;
		result[2].x = matrix.a3; result[2].y = matrix.b3; result[2].z = matrix.c3; result[2].w = matrix.d3;
		result[3].x = matrix.a4; result[3].y = matrix.b4; result[3].z = matrix.c4; result[3].w = matrix.d4;

		return result;
	}

	Model::Model()
		: m_Name(""), m_Filepath(""), m_Scene(nullptr)
	{
	}

	Model::Model(const std::string& filepath)
	{
		m_Name = filepath.substr(filepath.find_last_of("\\/") + 1, filepath.length() - filepath.find_last_of("\\/") - 1);
		m_Filepath = filepath.substr(0, filepath.find_last_of("\\/"));

		LoadFromFile(filepath);
	}

	Model::~Model()
	{
		if(m_Scene)
			m_Importer.FreeScene();
	}

	void Model::Render()
	{
		for (auto& mesh : m_Meshes)
			mesh->Render();
	}

	void Model::LoadFromFile(const std::string& filepath)
	{
		m_Scene = m_Importer.ReadFile(filepath, aiProcess_CalcTangentSpace |
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

		E3D_CORE_ASSERT(m_Scene, "Failed to load scene: {0}", m_Importer.GetErrorString());

		m_Meshes.reserve(m_Scene->mNumMeshes);

		for (size_t i = 0; i < m_Scene->mNumMeshes; i++)
			m_Meshes.push_back(ProcessMesh(m_Scene->mMeshes[i]));

		m_RootNode = ProcessNode(m_Scene->mRootNode);
	}

	void Model::AddMesh(const Ref<Mesh>& mesh)
	{
		E3D_CORE_ASSERT(mesh, "Mesh is null!");
		m_Meshes.push_back(mesh);
	}

	Ref<Mesh> Model::ProcessMesh(const aiMesh* mesh)
	{
		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
	
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			const aiVector3D position = mesh->mVertices[i];
			vertex.Position = glm::vec3(position.x, position.y, position.z);

			if (mesh->mTextureCoords[0])
			{
				const aiVector3D texCoord = mesh->mTextureCoords[0][i];
				vertex.TextureCoord = glm::vec2(texCoord.x, texCoord.y);
			}

			if (mesh->mNormals)
			{
				const aiVector3D normal = mesh->mNormals[i];
				vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
			}

			if (mesh->mTangents)
			{
				const aiVector3D tangent = mesh->mTangents[i];
				vertex.Tangent = glm::vec3(tangent.x, tangent.y, tangent.z);
			}

			if (mesh->mBitangents)
			{
				const aiVector3D bitangent = mesh->mBitangents[i];
				vertex.Bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
			}

			vertices.push_back(vertex);
		}
		
		std::vector<uint32_t> indices;
		indices.reserve(mesh->mNumFaces * 3);

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		Ref<Material> meshMaterial = CreateRef<Material>(ShaderLibrary::Get("StaticModelShader"));
		meshMaterial->SetName(std::string(mesh->mName.C_Str()) + "_Material");
		MaterialManager::LoadMaterial(meshMaterial);

		if (mesh->mMaterialIndex >= 0)
		{
			const aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];
			aiString textureName;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == aiReturn_SUCCESS)
			{
				std::string textureFilepath = m_Filepath + "/" + textureName.C_Str();
				meshMaterial->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create(textureFilepath)));
				meshMaterial->GetAlbedoMap()->GenerateMipMaps();
			}
			if (material->GetTexture(aiTextureType_NORMALS, 0, &textureName) == aiReturn_SUCCESS)
			{
				std::string textureFilepath = m_Filepath + "/" + textureName.C_Str();
				meshMaterial->SetNormalMap(Texture2D::Create(textureFilepath));
			}
			if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureName) == aiReturn_SUCCESS)
			{
				std::string textureFilepath = m_Filepath + "/" + textureName.C_Str();
				meshMaterial->SetRoughnessMap(Texture2D::Create(textureFilepath));
			}
			if (material->GetTexture(aiTextureType_METALNESS, 0, &textureName) == aiReturn_SUCCESS)
			{
				std::string textureFilepath = m_Filepath + "/" + textureName.C_Str();
				meshMaterial->SetMetalnessMap(Texture2D::Create(textureFilepath));
			}
		}

		Ref<Mesh> newMesh = CreateRef<Mesh>(vertices, indices, meshMaterial);
		newMesh->SetName(mesh->mName.C_Str());
		

		return newMesh;
	}

	Ref<ModelNode> Model::ProcessNode(const aiNode* node)
	{
		Ref<ModelNode> newNode = CreateRef<ModelNode>();

		newNode->Name = std::string(node->mName.C_Str());
		newNode->Transform = aiToGLM(node->mTransformation);
		
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			const uint32_t index = node->mMeshes[i];
			newNode->AddMesh(m_Meshes.at(index));
		}


		for (size_t i = 0; i < node->mNumChildren; i++)
			newNode->AddChild(ProcessNode(node->mChildren[i]));

		return newNode;
	}
}