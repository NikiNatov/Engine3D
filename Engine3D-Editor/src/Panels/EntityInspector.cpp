#include "EntityInspector.h"

#include "Engine3D\Scene\Scene.h"
#include "Engine3D\ResourceManager\ModelManager.h"

#include "Engine3D\Scene\Components.h"

#include <ImGui\imgui.h>
#include <ImGui\imgui_internal.h>
#include <ImGuizmo.h>

#include <glm\gtc\type_ptr.hpp>

namespace E3D
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename Type, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		auto& component = entity.GetComponent<Type>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		ImGui::SameLine(contentRegion.x - lineHeight * 0.5f);

		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			ImGui::OpenPopup("Component Options");

		ImGui::PopStyleVar();

		bool removeComponent = false;
		if (ImGui::BeginPopup("Component Options"))
		{
			if (ImGui::MenuItem("Remove"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<Type>();
	}

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

					if (componentData.Name == "Tag")
					{
						auto& tag = entity.GetComponent<NameComponent>();

						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), tag.Name.c_str());

						if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
						{
							tag.Name = std::string(buffer);
						}			
					}

					else if (componentData.Name == "Transform")
					{
						DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
						{
							DrawVec3Control("Transform", component.Translation);
							glm::vec3 rotation = glm::degrees(component.Rotation);
							DrawVec3Control("Rotation", rotation);
							component.Rotation = glm::radians(rotation);
							DrawVec3Control("Scale", component.Scale, 1.0f);
						});
					}
					else if (componentData.Name == "Mesh")
					{
						DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
						{
							ImGui::Columns(2);
							ImGui::SetColumnWidth(0, 100.0f);
							ImGui::Text("Mesh");
							ImGui::NextColumn();

							ImGui::InputText("##Mesh", (char*)component.Mesh->GetName().c_str(), 50, ImGuiInputTextFlags_ReadOnly);

							ImGui::Columns(1);
						});
					}
					else if (componentData.Name == "Camera")
					{
						DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
						{
							auto& camera = component.Camera;

							const char* projTypesStrings[] = { "Perspective", "Orthographic" };
							const char* currentProjType = projTypesStrings[(int)camera.GetProjectionType()];

							if (ImGui::BeginCombo("Projection", currentProjType))
							{
								for (int i = 0; i < 2; i++)
								{
									bool isSelected = currentProjType == projTypesStrings[i];
									if (ImGui::Selectable(projTypesStrings[i], isSelected))
									{
										currentProjType = projTypesStrings[i];
										camera.SetProjectionType((SceneCamera::ProjectionType)i);
									}

									if (isSelected)
										ImGui::SetItemDefaultFocus();
								}

								ImGui::EndCombo();
							}
							if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
							{		
								float fov = camera.GetPerspectiveFOV();
								float nearPlane = camera.GetPerspectiveNear();
								float farPlane = camera.GetPerspectiveFar();

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Perspective FOV");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##PerspectiveFOV", &fov))
									camera.SetPerspectiveFOV(fov);
								ImGui::Columns(1);

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Perspective Near");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##PerspectiveNear", &nearPlane))
									camera.SetPerspectiveNear(nearPlane);
								ImGui::Columns(1);

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Perspective Far");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##PerspectiveFar", &farPlane))
									camera.SetPerspectiveFar(farPlane);
								ImGui::Columns(1);
							}
							else
							{
								float orthoSize = camera.GetOrthographicSize();
								float orthoNear = camera.GetOrthographicNear();
								float orthoFar = camera.GetOrthographicFar();

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Orthographic Size");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##OrthographicSize", &orthoSize))
									camera.SetOrthographicSize(orthoSize);
								ImGui::Columns(1);

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Orthographic Near");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##OrthographicNear", &orthoNear))
									camera.SetOrthographicNear(orthoNear);
								ImGui::Columns(1);

								ImGui::Columns(2);
								ImGui::SetColumnWidth(0, 150.0f);
								ImGui::Text("Orthographic Far");
								ImGui::NextColumn();
								if (ImGui::DragFloat("##OrthographicFar", &orthoFar))
									camera.SetOrthographicFar(orthoFar);
								ImGui::Columns(1);
							}

							ImGui::Checkbox("Primary", &component.Primary);
						});
					}
					else if (componentData.Name == "Script")
					{
						DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
						{
						});
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