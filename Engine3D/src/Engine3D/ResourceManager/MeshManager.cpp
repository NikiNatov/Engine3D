#include "pch.h"
#include "MeshManager.h"

namespace E3D
{
	std::unordered_map<std::string, Ref<Mesh>> MeshManager::m_LoadedMeshes;

	Ref<Mesh> MeshManager::LoadMesh(const Ref<Mesh>& mesh)
	{
		const std::string& name = mesh->GetName();

		if (!Exists(name))
		{
			m_LoadedMeshes[name] = mesh;
			return mesh;
		}
		else
		{
			E3D_CORE_LOG_WARNING("Mesh with name \"{0}\" already exists", name);
			return m_LoadedMeshes[name];
		}
	}

	Ref<Mesh> MeshManager::GetMesh(const std::string& name)
	{
		E3D_CORE_ASSERT(Exists(name), "Mesh does not exist!");
		return m_LoadedMeshes[name];
	}

	bool MeshManager::Exists(const std::string& name)
	{
		return m_LoadedMeshes.find(name) != m_LoadedMeshes.end();
	}
}