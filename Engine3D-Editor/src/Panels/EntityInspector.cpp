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
							ImGui::BeginGroup();
							//ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Tag");
							ImGui::EndGroup();

							ImGui::SameLine(150.0f, 0.0f);

							ImGui::BeginGroup();
							if (ImGui::InputText("", buffer, sizeof(buffer)))
							{
								tag = std::string(buffer);
							}
							ImGui::EndGroup();

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
							ImGui::BeginGroup();
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Translation");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Rotation");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Scale");
							ImGui::EndGroup();

							ImGui::SameLine(150.0f, 0.0f);

							ImGui::BeginGroup();
							ImGui::DragFloat3("##Translation", &position.x, 0.5f, -1000.0f, 1000.0f);
							ImGui::DragFloat3("##Rotation", &rotation.x, 0.5f, -180.0f, 180.0f);
							ImGui::DragFloat3("##Scale", &scale.x, 0.5f, 0.5f, 100.0f);
							ImGui::EndGroup();

							ImGui::Separator();
						}

						ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
						
					}
					else if (componentData.Name == "Mesh")
					{
						auto& mesh = entity.GetComponent<MeshComponent>().Mesh;

						if (ImGui::CollapsingHeader("Mesh", flags))
						{
							ImGui::BeginGroup();
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("File Path");
							ImGui::EndGroup();

							ImGui::SameLine(150.0f, 0.0f);

							ImGui::BeginGroup();
							ImGui::InputText("", (char*)mesh->GetFilepath().c_str(), 60, ImGuiInputTextFlags_ReadOnly);
							ImGui::EndGroup();

							ImGui::Separator();
						}
					}
					else if (componentData.Name == "Camera")
					{
						auto& cameraComponent = entity.GetComponent<CameraComponent>();

						if (ImGui::CollapsingHeader("Camera", flags))
						{
							auto& camera = cameraComponent.Camera;
							float fov = camera.GetFov();
							float nearPlane = camera.GetNearPlane();
							float farPlane = camera.GetFarPlane();

							ImGui::BeginGroup();
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("FOV");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Near Plane");
							ImGui::Dummy(ImVec2{ 0.0f, 0.5f });
							ImGui::Text("Far Plane");
							ImGui::EndGroup();

							ImGui::SameLine(150.0f, 0.0f);

							ImGui::BeginGroup();
							if (ImGui::SliderFloat("##FOV", &fov, 0.0f, 180.0f))
								camera.SetFov(fov);
							if (ImGui::SliderFloat("##NearPlane", &nearPlane, 0.0f, 1000.0f))
								camera.SetNearPlane(nearPlane);
							if (ImGui::SliderFloat("##FarPlane", &farPlane, 0.0f, 1000.0f))
								camera.SetFarPlane(farPlane);
							ImGui::EndGroup();

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