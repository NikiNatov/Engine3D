#pragma once

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Scene.h"


namespace E3D
{
	struct ModelNode;

	class SceneGraph
	{
	public:
		SceneGraph() = default;
		SceneGraph(const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);

		void OnImGuiRender();

		inline Entity& GetSelectedEntity() { return m_SelectedEntity; }
	private:
		void DrawNode(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity = {};
	};
}