#pragma once
#include "Engine3D.h"

#include "Panels\SceneGraph.h"
#include "Panels\EntityInspector.h"
#include "Panels\ModelInspector.h"

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
		ShaderLibrary m_ShaderLibrary;

		Ref<Texture2D> m_PlayButtonTexture;
		Ref<Texture2D> m_StopButtonTexture;
		Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		
		Ref<Scene> m_Scene;

		bool m_PrimaryCamera = true;
		bool m_RunScene = false;

		Ref<Model> m_Model;
		Ref<Mesh> m_Grid;
		Ref<Skybox> m_Skybox;

		/*Entity m_Vader;
		Entity m_SpaceShip;
		Entity m_Terrain;*/
		Entity m_MainCamera;
		Entity m_Cube;
		Entity m_Plane;
		Entity m_Cone;
		Entity m_Sphere;
		Entity m_Pistol;

		EntityInspector m_InspectorPanel;
		SceneGraph m_SceneGraphPanel;
		ModelInspector m_ModelPanel;
		Entity m_SelectedEntity{};

		Ref<Material> m_Gold;
		Ref<Material> m_Grass;
		Ref<Material> m_Marble;
		Ref<Material> m_Rock;
		Ref<Material> m_RustedIron;
		Ref<Material> m_Wood;
		Ref<Material> m_PistolMaterial;
	};
}