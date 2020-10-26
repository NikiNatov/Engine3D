#pragma once

#include "Engine3D\Renderer\Mesh.h"

namespace E3D
{
	class MeshManager
	{
	public:
		MeshManager(const MeshManager& other) = delete;
		MeshManager& operator=(const MeshManager& other) = delete;

		static Ref<Mesh> LoadMesh(const Ref<Mesh>& mesh);
		
		static Ref<Mesh> GetMesh(const std::string& name);
		static const std::unordered_map<std::string, Ref<Mesh>>& GetLoadedMeshes() { return m_LoadedMeshes; }
		static bool Exists(const std::string& name);
	private:
		MeshManager() = default;
	private:
		static std::unordered_map<std::string, Ref<Mesh>> m_LoadedMeshes;
	};
}