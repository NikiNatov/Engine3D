#include "pch.h"
#include "MaterialManager.h"

namespace E3D
{
	std::unordered_map<std::string, Ref<Material>> MaterialManager::m_LoadedMaterials;

	Ref<Material> MaterialManager::LoadMaterial(const Ref<Material>& material)
	{
		const std::string& name = material->GetName();

		if (!Exists(name))
		{
			m_LoadedMaterials[name] = material;
			return material;
		}
		else
		{
			E3D_CORE_LOG_WARNING("Material with name \"{0}\" already exists", name);
			return m_LoadedMaterials[name];
		}
	}

	Ref<Material> MaterialManager::GetMaterial(const std::string& name)
	{
		E3D_CORE_ASSERT(Exists(name), "Material does not exist!");
		return m_LoadedMaterials[name];
	}

	bool MaterialManager::Exists(const std::string& name)
	{
		return m_LoadedMaterials.find(name) != m_LoadedMaterials.end();
	}
}