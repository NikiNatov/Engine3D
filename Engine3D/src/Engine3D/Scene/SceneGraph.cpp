#include "pch.h"
#include "SceneGraph.h"

#include "Engine3D\Scene\Scene.h"

#include <imgui.h>

namespace E3D
{
	SceneGraph::SceneGraph(Scene* scene)
	{
		Init(scene);
	}

	void SceneGraph::Init(Scene* scene)
	{
		m_Scene = scene;

		if(!m_Root)
			m_Root = m_Scene->CreateEntity("World");
	}


	void SceneGraph::DisplaySceneHierarchy()
	{
		ImGui::Begin("Scene Graph");
		m_Root->DisplayTree(m_SelectedEntity);
		ImGui::End();

		ImGui::Begin("Entity Properties");

		if (m_SelectedEntity)
			m_SelectedEntity->DisplayEntityProperties();

		ImGui::End();
	}

}