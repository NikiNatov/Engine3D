#include "ModelInspector.h"
#include "Engine3D\Scene\Components.h"

#include <ImGui\imgui.h>

namespace E3D
{
	ModelInspector::ModelInspector()
	{
		m_WhiteTexture = Texture2D::Create("assets/textures/checkerboard.png");
		m_WhiteTexture->GenerateMipMaps();
		m_Normal = Texture2D::Create("assets/models/gun/Textures/Cerberus_N.tga");
		m_Normal->GenerateMipMaps();
		m_Roughness = Texture2D::Create("assets/models/gun/Textures/Cerberus_R.tga");
		m_Roughness->GenerateMipMaps();
		m_Metalness = Texture2D::Create("assets/models/gun/Textures/Cerberus_M.tga");
		m_Metalness->GenerateMipMaps();
	}

	void ModelInspector::OnImGuiRender(Entity entity)
	{
		ImGui::Begin("Model");
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		
		if (entity)
		{
			if (entity.HasComponent<MeshComponent>())
			{
				auto& mesh = entity.GetComponent<MeshComponent>().Mesh;

				if (mesh)
				{
					ImGui::Text(mesh->GetFilepath().c_str());

					auto& material = mesh->GetMeshes()[0]->GetMaterial();

					m_Normal = material->GetNormalMap();
					m_Metalness = material->GetMetalnessMap();
					m_Roughness = material->GetRoughnessMap();
					/*material->SetNormalMap(m_Normal);
					material->SetMetalnessMap(m_Metalness);
					material->SetRoughnessMap(m_Roughness);*/

					if (ImGui::CollapsingHeader("Albedo", flags))
					{
						uint32_t id = material->m_Properties.AlbedoMap ? material->m_Properties.AlbedoMap->GetTextureID() : m_WhiteTexture->GetTextureID();
						ImGui::ImageButton((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
						ImGui::SameLine();
						ImGui::BeginGroup();

						ImGui::Checkbox("Use##Albedo", &material->m_UseAlbedoMap);
						ImGui::SameLine();
						ImGui::ColorEdit3("", &material->m_Properties.Albedo.r, ImGuiColorEditFlags_NoInputs);

						ImGui::EndGroup();
					}

					if (ImGui::CollapsingHeader("Normal", flags))
					{
						uint32_t id = material->m_Properties.NormalMap ? material->m_Properties.NormalMap->GetTextureID() : m_WhiteTexture->GetTextureID();
						ImGui::ImageButton((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
						ImGui::SameLine();
						ImGui::BeginGroup();

						ImGui::Checkbox("Use##Normal", &material->m_UseNormalMap);

						ImGui::EndGroup();
					}

					if (ImGui::CollapsingHeader("Roughness", flags))
					{
						uint32_t id = material->m_Properties.RoughnessMap ? material->m_Properties.RoughnessMap->GetTextureID() : m_WhiteTexture->GetTextureID();
						ImGui::ImageButton((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

						ImGui::SameLine();
						ImGui::BeginGroup();

						ImGui::Checkbox("Use##Roughness", &material->m_UseRoughnessMap);
						ImGui::SameLine();
						ImGui::DragFloat("##Roughness", &material->m_Properties.Roughness, 0.01f, 0.0f, 1.0f);

						ImGui::EndGroup();
					}

					if (ImGui::CollapsingHeader("Metalness", flags))
					{
						uint32_t id = material->m_Properties.MetalnessMap ? material->m_Properties.MetalnessMap->GetTextureID() : m_WhiteTexture->GetTextureID();
						ImGui::ImageButton((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

						ImGui::SameLine();
						ImGui::BeginGroup();

						ImGui::Checkbox("Use##Metalness", &material->m_UseMetalnessMap);
						ImGui::SameLine();
						ImGui::DragFloat("##Metalness", &material->m_Properties.Metalness, 0.01f, 0.0f, 1.0f);

						ImGui::EndGroup();
					}
				}
				else
				{

					uint32_t id = m_WhiteTexture->GetTextureID();
					if (ImGui::CollapsingHeader("Albedo", flags))
					{
						ImGui::Image((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::CollapsingHeader("Normal", flags))
					{
						ImGui::Image((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::CollapsingHeader("Roughness", flags))
					{
						ImGui::Image((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::CollapsingHeader("Metalness"), flags)
					{
						ImGui::Image((void*)id, ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
					}
				}
			}
		}
		ImGui::End();
	}
}