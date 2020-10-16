#pragma once

#include "Engine3D\Renderer\Material.h"

namespace E3D
{
	class MaterialManager
	{
	public:
		MaterialManager(const MaterialManager& other) = delete;
		MaterialManager& operator=(const MaterialManager& other) = delete;

		static Ref<Material> LoadMaterial(const Ref<Material>& material);

		static Ref<Material> GetMaterial(const std::string& name);
		static const std::unordered_map<std::string, Ref<Material>>& GetLoadedMaterials() { return m_LoadedMaterials; }
		static bool Exists(const std::string& name);
	private:
		MaterialManager() {};
	private:
		static std::unordered_map<std::string, Ref<Material>> m_LoadedMaterials;
	};
}