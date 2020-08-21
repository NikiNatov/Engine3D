#pragma once

#include "Engine3D\Scene\Entity.h"

namespace E3D
{

	class SceneGraph
	{
	public:
		SceneGraph() = default;
		SceneGraph(Scene* scene);

		void Init(Scene* scene);

		void DisplaySceneHierarchy();

		inline Ref<Entity>& GetRoot() { return m_Root; }
	private:
		Ref<Entity> m_Root;
		Scene* m_Scene = nullptr;
		Entity* m_SelectedEntity = nullptr;
	};
}