#pragma once
#include "Engine3D.h"

#include "Panels\SceneGraph.h"
#include "Panels\EntityInspector.h"
#include "Panels\MaterialInspector.h"
#include "Panels\AssetsPanel.h"


namespace E3D
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;

		virtual void OnImGuiRender() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void LoadAssets();
		void LoadModels();
		void LoadTextures();
		void LoadShaders();

		void SaveSceneAs();
		void NewScene();
		void OpenScene();
	private:
		Ref<Texture2D> m_PlayButtonTexture;
		Ref<Texture2D> m_StopButtonTexture;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_ShadowFB;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		
		Ref<Scene> m_Scene;

		bool m_PrimaryCamera = true;
		bool m_RunScene = false;

		Ref<Mesh> m_Grid;
		Ref<Skybox> m_Skybox;

		Entity m_MainCamera;
		Entity m_Player;

		EntityInspector m_InspectorPanel;
		SceneGraph m_SceneGraphPanel;
		MaterialInspector m_MaterialPanel;
		AssetsPanel m_AssetsPanel;

		Entity m_SelectedEntity{};
		int m_GuizmoOperation = 0;
		bool m_GuizmoSnap = false;

		Ref<Material> m_Gold;
		Ref<Material> m_Grass;
		Ref<Material> m_Marble;
		Ref<Material> m_Rock;
		Ref<Material> m_RustedIron;
		Ref<Material> m_Wood;
		Ref<Material> m_PistolMaterial;
		Ref<Material> m_TestMat;

		
	};
}