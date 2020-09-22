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
			

			std::unordered_map<ComponentID, ComponentData> missingComponents;
			
			for (auto& [componentID, componentData] : m_RegisteredComponents)
			{
				
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

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

					if (componentData.Name == "Tag")
					{
						auto& tag = entity.GetComponent<NameComponent>().Name;

						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), tag.c_str());
						if (ImGui::CollapsingHeader("Tag", flags))
						{
							ImGui::Columns(2);
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Tag");
							ImGui::NextColumn();
							if (ImGui::InputText("", buffer, sizeof(buffer)))
							{
								tag = std::string(buffer);
							}
							ImGui::Columns(1);
							ImGui::Separator();
						}
					}

					else if (componentData.Name == "Transform")
					{
						auto& transform = entity.GetComponent<TransformComponent>().Transform;
						glm::vec3 position, scale;
						glm::vec3 rotation;
						ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));

						if (ImGui::CollapsingHeader("Transform", flags))
						{
							ImGui::Columns(2);
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Translation");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Rotation");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Scale");

							ImGui::NextColumn();
							ImGui::DragFloat3("##Translation", &position.x, 0.5f, -1000.0f, 1000.0f);
							ImGui::DragFloat3("##Rotation", &rotation.x, 0.5f, -180.0f, 180.0f);
							ImGui::DragFloat3("##Scale", &scale.x, 0.5f, 0.5f, 100.0f);

							ImGui::Columns(1);
							ImGui::Separator();
						}

						ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
						
					}
					else if (componentData.Name == "Mesh")
					{
						auto& mesh = entity.GetComponent<MeshComponent>().Mesh;

						if (ImGui::CollapsingHeader("Mesh", flags))
						{
							ImGui::Columns(2);
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("File Path");
							ImGui::NextColumn();
							ImGui::InputText("", (char*)mesh->GetFilepath().c_str(), 60, ImGuiInputTextFlags_ReadOnly);

							ImGui::Columns(1);
							ImGui::Separator();
						}
					}
					else if (componentData.Name == "Camera")
					{
						auto& cameraComponent = entity.GetComponent<CameraComponent>();

						if (ImGui::CollapsingHeader("Camera", flags))
						{
							auto& cameraComponent = entity.GetComponent<CameraComponent>();

							ImGui::Checkbox("Primary", &cameraComponent.Primary);
						}
					}
					else if (componentData.Name == "Script")
					{
						if (ImGui::CollapsingHeader("Script", flags))
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
		}
		ImGui::End();
	}
}