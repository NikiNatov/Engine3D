#pragma once

#include "Engine3D\Scene\Scene.h"

namespace E3D
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filpath);
		bool DeserializeRuntime(const std::string& filpath);
	private:
		Ref<Scene> m_Scene;
	};
}