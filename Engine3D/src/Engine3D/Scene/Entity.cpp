#include "pch.h"
#include "Entity.h"

#include "Engine3D\Scene\Components.h"
#include "Engine3D\Scene\Scene.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

namespace E3D
{
	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{
	}

	void Entity::AddChild(Ref<Entity>& child)
	{
		E3D_CORE_ASSERT(child, "Child is null!");
		m_Children.push_back(child);
	}

	void Entity::RemoveChild(Ref<Entity>& child)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), child);

		if (it != m_Children.end())
			m_Children.erase(it);
	}

	void Entity::DisplayTree(Entity*& selectedEntity)
	{
		if (m_Scene->m_Registry.valid(m_Entity))
		{
			bool hasName = HasComponent<NameComponent>();

			const auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | (m_Children.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

			bool isTreeOpen = ImGui::TreeNodeEx(hasName ? GetComponent<NameComponent>().Name.c_str() : "Unnamed Entity", nodeFlags);

			ImGui::PushID(entt::to_integral(m_Entity));
			if (ImGui::BeginPopupContextItem("Options Menu", 1))
			{
				if (ImGui::Selectable("Add Child"))
				{
					m_Children.push_back(m_Scene->CreateEntity());
				}
				if (ImGui::Selectable("Delete"))
				{
					m_Scene->m_Registry.destroy(m_Entity);
					// TODO: Remove from the tree also
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

			if (isTreeOpen)
			{
				if (ImGui::IsItemClicked())
					selectedEntity = this;

				for (const auto& child : m_Children)
					child->DisplayTree(selectedEntity);

				ImGui::TreePop();
			}
		}
	}


	void Entity::DisplayEntityProperties()
	{
		if (m_Scene->m_Registry.valid(m_Entity))
		{
			if (HasComponent<NameComponent>())
				ImGui::InputText("Tag", (char*)GetComponent<NameComponent>().Name.c_str(), 25);

			if (HasComponent<TransformComponent>())
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				glm::vec3 position, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));

				if (ImGui::CollapsingHeader("Transform"))
				{
					ImGui::DragFloat3("Translation", &position.x, 0.5f, -1000.0f, 1000.0f);
					ImGui::DragFloat3("Rotation", &rotation.x, 0.5f, -180.0f, 180.0f);
					ImGui::DragFloat3("Scale", &scale.x, 0.5f, 0.5f, 3.0f);
				}

				ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
			}

			if (HasComponent<CameraComponent>())
			{
				auto& cameraComponent = GetComponent<CameraComponent>();

				if (ImGui::CollapsingHeader("Camera"))
				{

				}
			}

			if (HasComponent<MeshComponent>())
			{
				auto& mesh = GetComponent<MeshComponent>().Mesh;
				if (ImGui::CollapsingHeader("Mesh"))
				{
					ImGui::InputText("", (char*)mesh->GetFilepath().c_str(), 50, ImGuiInputTextFlags_ReadOnly);
				}
			}
		}
	}

	void Entity::Render(const glm::mat4& parentTransform)
	{
		auto& name = GetComponent<NameComponent>().Name;
		if (name == "World")
		{
			for (const auto& child : m_Children)
				child->Render(parentTransform);

			return;
		}

		if (HasComponent<TransformComponent>() && HasComponent<MeshComponent>())
		{
			auto [transformComponent, meshComponent] = m_Scene->m_Registry.get<TransformComponent, MeshComponent>(m_Entity);

			glm::mat4 finalTransform = parentTransform * transformComponent.Transform;

			meshComponent.Mesh->Draw(finalTransform);

			for (const auto& child : m_Children)
				child->Render(finalTransform);
		}
	}
}