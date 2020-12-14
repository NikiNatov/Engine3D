#include "SceneGraph.h"

#include "Engine3D\Scene\Components.h"
#include "Engine3D\Renderer\Model.h"

#include "Engine3D\Renderer\MeshFactory.h"

#include <ImGui\imgui.h>

#include "Engine3D\ResourceManager\ModelManager.h"
#include "Engine3D\ResourceManager\MeshManager.h"
#include "Engine3D\Core\FileDialog.h"

namespace E3D
{
	SceneGraph::SceneGraph(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void SceneGraph::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectedEntity = {};
	}

	void SceneGraph::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity(entityID, m_Scene.get());

			auto& parent = entity.GetComponent<SceneNodeComponent>().Parent;

			if (!parent)
				DrawNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("New Entity"))
			{
				Entity newEntity = m_Scene->CreateEntity();
				newEntity.AddComponent<MeshComponent>(MeshManager::LoadMesh(MeshFactory::CreatePlane({ 1.0f, 1.0f }, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader")))));
			}
			if (ImGui::MenuItem("New Model"))
			{
				const std::string& path = FileDialog::OpenModelFile();

				if (!path.empty())
				{
					auto& newModel = ModelManager::LoadModel(path);
					m_Scene->CreateFromModel(newModel, newModel->GetName());
				}
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneGraph::DrawNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;
		auto& nodeComponent = entity.GetComponent<SceneNodeComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);

		bool isOpen;
		if (entity.HasComponent<MeshComponent>())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f });
			isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
			ImGui::PopStyleColor();
		}
		else
			isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
			m_SelectedEntity = entity;

		ImGui::PushID((void*)(uint64_t)(uint32_t)entity);


		bool removeEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			m_SelectedEntity = entity;

			if (ImGui::BeginMenu("Add type"))
			{
				if (ImGui::MenuItem("Empty entity"))
				{
					Entity newEntity = m_Scene->CreateEntity();
						newEntity.AddComponent<MeshComponent>(MeshFactory::CreateCube(1.0f, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader"))));
						entity.AddChild(newEntity);
				}
				if (ImGui::MenuItem("Model entity"))
				{
					const std::string& path = FileDialog::OpenModelFile();

						if (!path.empty())
						{
							auto& newModel = ModelManager::LoadModel(path);
							Entity newEntity = m_Scene->CreateFromModel(newModel, newModel->GetName());
							entity.AddChild(newEntity);
						}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Remove Entity"))
			{
				removeEntity = true;
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

		if (isOpen)
		{
			Entity currentChild = entity.GetComponent<SceneNodeComponent>().FirstChild;

			Entity nextSibling;
			if(currentChild)
				nextSibling = currentChild.GetComponent<SceneNodeComponent>().NextSibling;

			while (currentChild)
			{
				DrawNode(currentChild);
				currentChild = nextSibling;
				if (nextSibling)
					nextSibling = nextSibling.GetComponent<SceneNodeComponent>().NextSibling;
			}

			ImGui::TreePop();
		}

		if (removeEntity)
		{
			if(m_SelectedEntity == entity)
				m_SelectedEntity = {};
			m_Scene->DeleteEntity(entity);
		}
	
	}

}