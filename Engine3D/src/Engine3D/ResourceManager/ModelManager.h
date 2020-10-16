#pragma once

#include "Engine3D\Renderer\Model.h"

namespace E3D
{
	class ModelManager
	{
	public:
		ModelManager(const ModelManager& other) = delete;
		ModelManager& operator=(const ModelManager& other) = delete;

		static Ref<Model> LoadModel(const std::string& filepath);

		static Ref<Model> GetModel(const std::string& filepath);
		static const std::unordered_map<std::string, Ref<Model>>& GetLoadedModels() { return m_LoadedModels; }
		static bool Exists(const std::string& filepath);
	private:
		ModelManager() {};
	private:
		static std::unordered_map<std::string, Ref<Model>> m_LoadedModels;

	};
}