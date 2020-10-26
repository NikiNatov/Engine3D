#include "AssetsPanel.h"

#include "Engine3D\ResourceManager\ModelManager.h"
#include "Engine3D\ResourceManager\TextureManager.h"
#include "Engine3D\ResourceManager\MaterialManager.h"
#include "Engine3D\ResourceManager\MeshManager.h"

#include <ImGui\imgui.h>

namespace E3D
{
	void AssetsPanel::OnImGuiRender()
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
		for (auto& element : MeshManager::GetLoadedMeshes())
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
