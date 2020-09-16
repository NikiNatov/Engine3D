#include "SceneGraph.h"

#include "Engine3D\Scene\Components.h"

#include <ImGui\imgui.h>

namespace E3D
{
	SceneGraph::SceneGraph(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void SceneGraph::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
	}

	void SceneGraph::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// Temporary //////////////////////////////////////////////////////////////////////
		if (ImGui::Button("Create Entity"))
		{

			Entity newEntity = m_Scene->CreateEntity();
			newEntity.AddComponent<MeshComponent>("assets/models/primitives/cube.fbx");
		}
		///////////////////////////////////////////////////////////////////////////////////

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity(entityID, m_Scene.get());

			auto& parent = entity.GetComponent<SceneNodeComponent>().Parent;

			if (!parent)
				DrawNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::End();
	}

	void SceneGraph::DrawNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;
		auto& nodeComponent = entity.GetComponent<SceneNodeComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);

		bool isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
			m_SelectedEntity = entity;

		ImGui::PushID((void*)(uint64_t)(uint32_t)entity);
		if (ImGui::BeginPopupContextItem("Options Menu", 1))
		{
			m_SelectedEntity = entity;
			if (ImGui::Selectable("Add Child"))
			{
				if (!nodeComponent.FirstChild)
				{
					entity.GetComponent<SceneNodeComponent>().FirstChild = m_Scene->CreateEntity();
					entity.GetComponent<SceneNodeComponent>().FirstChild.GetComponent<SceneNodeComponent>().Parent = m_SelectedEntity;
				}
				else
				{
					auto currentChild = nodeComponent.FirstChild;

					while (currentChild.GetComponent<SceneNodeComponent>().NextSibling)
						currentChild = currentChild.GetComponent<SceneNodeComponent>().NextSibling;

					currentChild.GetComponent<SceneNodeComponent>().NextSibling = m_Scene->CreateEntity();
					currentChild.GetComponent<SceneNodeComponent>().NextSibling.GetComponent<SceneNodeComponent>().Parent = m_SelectedEntity;
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

		if (isOpen)
		{
			auto currentChild = nodeComponent.FirstChild;

			while (currentChild)
			{
				DrawNode(currentChild);
				currentChild = currentChild.GetComponent<SceneNodeComponent>().NextSibling;
			}

			ImGui::TreePop();
		}
	}

}