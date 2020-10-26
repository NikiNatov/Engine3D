#include "pch.h"
#include "ModelManager.h"

#include "Engine3D\ResourceManager\MeshManager.h"

namespace E3D
{
	std::unordered_map<std::string, Ref<Model>> ModelManager::m_LoadedModels;

	Ref<Model> ModelManager::LoadModel(const std::string& filepath)
	{
		if (!Exists(filepath))
		{
			Ref<Model> newModel = CreateRef<Model>(filepath);
			m_LoadedModels[filepath] = newModel;

			const auto& meshes = newModel->GetMeshList();
			for (const auto& mesh : meshes)
				MeshManager::LoadMesh(mesh);

			return newModel;
		}
		else
		{
			E3D_CORE_LOG_WARNING("Model with filepath \"{0}\" already exists", filepath);
			return m_LoadedModels[filepath];
		}
	}

	Ref<Model> ModelManager::GetModel(const std::string& filepath)
	{
		E3D_CORE_ASSERT(Exists(filepath), "Model does not exist!");
		return m_LoadedModels[filepath];
	}

	bool ModelManager::Exists(const std::string& filepath)
	{
		return m_LoadedModels.find(filepath) != m_LoadedModels.end();
	}
}