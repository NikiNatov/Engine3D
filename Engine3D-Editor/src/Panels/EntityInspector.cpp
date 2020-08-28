#include "EntityInspector.h"

#include "Engine3D\Scene\Scene.h"

#include <ImGui\imgui.h>
#include <ImGuizmo.h>

#include <glm\gtc\type_ptr.hpp>

namespace E3D
{

	EntityInspector::EntityInspector()
	{
		RegisterComponent<NameComponent>("Tag");
		RegisterComponent<TransformComponent>("Transform");
		RegisterComponent<MeshComponent>("Mesh");
		RegisterComponent<CameraComponent>("Camera");
		RegisterComponent<ScriptComponent>("Script");
	}

	void EntityInspector::OnImGuiRender(Entity entity)
	{
		ImGui::Begin("Entity Inspector");

		if (entity)
		{
			ImGui::PushID((uint32_t)entity);
			ImGui::InputText("Tag", (char*)entity.GetComponent<NameComponent>().Name.c_str(), 30);
			ImGui::Separator();

			std::unordered_map<ComponentID, ComponentData> missingComponents;
			
			for (auto& [componentID, componentData] : m_RegisteredComponents)
			{
				if (componentData.Name == "Tag")
					continue;
				if (entity.HasComponent(componentID))
				{
					ImGui::PushID(componentID);

					if (ImGui::Button("x"))
					{
						componentData.RemoveComponent(entity);
						ImGui::PopID();
						continue;
					}

					ImGui::SameLine();

					if (componentData.Name == "Transform")
					{
						auto& transform = entity.GetComponent<TransformComponent>().Transform;
						glm::vec3 position, scale;
						glm::vec3 rotation;
						ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));

						if (ImGui::CollapsingHeader("Transform"))
						{
							ImGui::DragFloat3("Translation", &position.x, 0.5f, -1000.0f, 1000.0f);
							ImGui::DragFloat3("Rotation", &rotation.x, 0.5f, -180.0f, 180.0f);
							ImGui::DragFloat3("Scale", &scale.x, 0.5f, 0.5f, 100.0f);
						}

						ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
					}
					else if (componentData.Name == "Mesh")
					{
						auto& mesh = entity.GetComponent<MeshComponent>().Mesh;

						if (ImGui::CollapsingHeader("Mesh"))
							ImGui::InputText("", (char*)mesh->GetFilepath().c_str(), 60, ImGuiInputTextFlags_ReadOnly);
					}
					else if (componentData.Name == "Camera")
					{
						auto& cameraComponent = entity.GetComponent<CameraComponent>();

						if (ImGui::CollapsingHeader("Camera"))
						{
							auto& cameraComponent = entity.GetComponent<CameraComponent>();

							ImGui::Checkbox("Primary", &cameraComponent.Primary);
						}
					}
					else if (componentData.Name == "Script")
					{
						if (ImGui::CollapsingHeader("Script"))
						{
						}
					}


					ImGui::PopID();
				}
				else
				{
					missingComponents[componentID] = componentData;
				}
			}

			if (!missingComponents.empty())
			{
				if (ImGui::Button("Add"))
					ImGui::OpenPopup("Add Component");

				if (ImGui::BeginPopup("Add Component"))
				{
					for (auto& [componentID, componentData] : missingComponents)
					{
						ImGui::PushID(componentID);

						if (ImGui::Selectable(componentData.Name.c_str()))
							componentData.AddComponent(entity);

						ImGui::PopID();
					}

					ImGui::EndPopup();
				}
			}

			ImGui::PopID();
		}
		ImGui::End();
	}
}