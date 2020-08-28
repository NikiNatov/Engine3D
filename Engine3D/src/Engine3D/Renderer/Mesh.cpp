#include "pch.h"
#include "Mesh.h"

#include <imgui.h>

#include "Engine3D\Renderer\Buffer.h"
#include "Engine3D\Renderer\Renderer.h"

#include <assimp/postprocess.h>
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

	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		BufferLayout layout = {
			{"a_Position",	DataType::Float3, false},
			{"a_TexCoord",	DataType::Float2, false},
			{"a_Normal",	DataType::Float3, false},
			{"a_Tangent",	DataType::Float3, false},
			{"a_Bitangent",	DataType::Float3, false}
		};

		m_VAO = VertexArray::Create();
		m_VAO->Bind();

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		vbo->SetLayout(layout);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), indices.size());

		m_VAO->AddVertexBuffer(vbo);
		m_VAO->SetIndexBuffer(ibo);


		m_Material = CreateRef<Material>(Shader::Create("assets/shaders/MaterialShader.glsl"));
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::ShowMeshWindow()
	{
		ImGui::Begin("Mesh");

		ImGui::Separator();

		bool diffuseTex = m_Material->HasDiffuseTexture();
		bool specularTex = m_Material->HasSpecularTexture();

		ImGui::Text("Diffuse Texture");
		if (m_Material->HasDiffuseTexture())
		{
			ImGui::Image((void*)m_Material->GetDiffuseTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Checkbox("Use##ModelDiffuseTexture", &diffuseTex))
				m_Material->UseDiffuseTexture(diffuseTex);
			ImGui::SameLine();
			ImGui::ColorEdit3("", &m_Material->GetDiffuseColor().x, ImGuiColorEditFlags_NoInputs);
			ImGui::EndGroup();
		}
		//else
			//ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

		ImGui::Text("Specular Texture");
		if (m_Material->HasSpecularTexture())
		{
			ImGui::Image((void*)m_Material->GetSpecularTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Checkbox("Use##ModelSpecularTexture", &specularTex))
				m_Material->UseSpecularTexture(true);

			ImGui::SameLine();
			ImGui::ColorEdit3("", &m_Material->GetSpecularColor().x, ImGuiColorEditFlags_NoInputs);
			ImGui::EndGroup();
		}
		//else
		//	ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

		ImGui::DragFloat("Shininess", &m_Material->GetShininess(), 0.5f, 0.0f, 180.0f);
		ImGui::DragFloat("Transparency", &m_Material->GetTransparency(), 0.03f, 0.0f, 1.0f);
		ImGui::End();
	}

	Model::Model(const std::string& filepath)
		: m_Filepath(filepath)
	{
		m_Scene = m_Importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		E3D_CORE_ASSERT(m_Scene, "Failed to load the model file!");

		for (size_t i = 0; i < m_Scene->mNumMeshes; i++)
		{
			m_Meshes.push_back(ParseMesh(m_Scene->mMeshes[i]));
		}

		m_Root = ParseNode(m_Scene->mRootNode);
	}

	Model::~Model()
	{
		
	}

	void Model::Draw(const glm::mat4& transform)
	{
		m_Root->Draw(transform);
	}

	Ref<Mesh> Model::ParseMesh(const aiMesh* mesh)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve((size_t)(mesh->mNumFaces * 3));

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

			if (mesh->mTextureCoords[0])
				vertex.TextureCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else
				vertex.TextureCoord = { 0.0f, 0.0f };

			if (mesh->mNormals)
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			else
				vertex.Normal = { 0.0f, 0.0f, 0.0f };

			if (mesh->mTangents)
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			else
				vertex.Tangent = { 0.0f, 0.0f, 0.0f };

			if (mesh->mBitangents)
				vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			else
				vertex.Bitangent = { 0.0f, 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		Ref<Mesh> newMesh = CreateRef<Mesh>(vertices, indices);

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];
			auto& newMeshMaterial = newMesh->GetMaterial();

			aiString name;
			aiColor3D diffuse, ambient, specular;
			ai_real shininess = 1.0f, transparency = 1.0f;
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_NAME, name)));
				//E3D_CORE_ASSERT(false, "Failed to load material name!");
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambient)));
				//E3D_CORE_ASSERT(false, "Failed to load material ambient color!");
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)));
				//E3D_CORE_ASSERT(false, "Failed to load material diffuse color!");
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specular)));
				//E3D_CORE_ASSERT(false, "Failed to load material specular color!");
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_OPACITY, transparency)));
				//E3D_CORE_ASSERT(false, "Failed to load material transparency!");
			if (!(AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess)));
				//E3D_CORE_ASSERT(false, "Failed to load material shininess!");

			newMeshMaterial->SetName(std::string(name.C_Str()));
			newMeshMaterial->SetAmbientColor({ ambient.r, ambient.g, ambient.b });
			newMeshMaterial->SetDiffuseColor({ diffuse.r, diffuse.g, diffuse.b });
			newMeshMaterial->SetSpecularColor({ specular.r, specular.g, specular.b });
			newMeshMaterial->SetTransparency(transparency);
			newMeshMaterial->SetShininess(shininess);

			aiString textureFileName;
			std::string textureDirectory = m_Filepath.substr(0, m_Filepath.find_last_of('/'));
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName);

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName) == aiReturn_SUCCESS)
			{
				Ref<Texture> diffuseTexture = Texture2D::Create(textureDirectory + "/" + textureFileName.C_Str());
				newMeshMaterial->SetDiffuseTexture(diffuseTexture);
			}

			if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFileName) == aiReturn_SUCCESS)
			{
				Ref<Texture> specularTexture = Texture2D::Create(textureDirectory + "/" + textureFileName.C_Str());
				newMeshMaterial->SetSpecularTexture(specularTexture);
			}

			if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFileName) == aiReturn_SUCCESS)
			{
				Ref<Texture> normalMap = Texture2D::Create(textureDirectory + "/" + textureFileName.C_Str());
				newMeshMaterial->SetNormalMap(normalMap);
			}

		}

		return newMesh;
	}
	Ref<Node> Model::ParseNode(const aiNode* node)
	{

		std::vector<Ref<Mesh>> currentNodeMeshes;

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			const auto meshIndex = node->mMeshes[i];
			currentNodeMeshes.push_back(m_Meshes.at(meshIndex));
		}

		glm::mat4 nodeTransform = aiToGLM(node->mTransformation);
		auto newNode = CreateRef<Node>(node->mName.C_Str(), currentNodeMeshes, nodeTransform);

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			newNode->AddChild(ParseNode(node->mChildren[i]));
		}

		return newNode;
	}

	Node::Node(const std::string& name, const std::vector<Ref<Mesh>>& meshes, const glm::mat4& transform)
		: m_Meshes(meshes), m_Transform(transform), m_Name(name)
	{
	}

	void Node::AddChild(const Ref<Node>& child)
	{
		E3D_CORE_ASSERT(child, "Child node is null!");
		m_Children.push_back(child);
	}

	void Node::RenderTree(Node*& selectedNode)
	{
		const auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | (selectedNode == this ? ImGuiTreeNodeFlags_Selected : 0)
			| (m_Children.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

		bool isTreeOpen = ImGui::TreeNodeEx(m_Name.c_str(), nodeFlags, m_Name.c_str());

		if (isTreeOpen)
		{
			if (ImGui::IsItemClicked())
				selectedNode = this;

			for (const auto& child : m_Children)
				child->RenderTree(selectedNode);
			ImGui::TreePop();
		}
	}

	void Node::Draw(const glm::mat4& parentTransform) const
	{
		glm::mat4 finalTransform = parentTransform * m_Transform;

		for (const auto& mesh : m_Meshes)
			Renderer::Submit(mesh, finalTransform);

		for (const auto& child : m_Children)
			child->Draw(finalTransform);
	}
}
