#include "AssetsPanel.h"

#include <ImGui\imgui.h>

namespace E3D
{
	void E3D::AssetsPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		ImGui::BeginGroup();
		for (auto& element : MaterialManager::GetLoadedMaterials())
			ImGui::Text(element.second->GetName().c_str());
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		for (auto& element : ModelManager::GetLoadedModels())
			ImGui::Text(element.second->GetName().c_str());
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		for (auto& element : TextureManager::GetLoadedTextures())
		{
			ImGui::BeginGroup();
			ImGui::Image((void*)element.second->GetTextureID(), { element.second->GetWidth() / 32.0f, element.second->GetHeight() / 32.0f }, { 0, 1 }, { 1, 0 });
			ImGui::Text(element.second->GetName().c_str());
			ImGui::EndGroup();
		}
		ImGui::EndGroup();

		ImGui::End();
	}
}
