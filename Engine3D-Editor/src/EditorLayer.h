#pragma once
#include "Engine3D.h"

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

		PerspectiveCameraController m_CameraController{ 45.0f, (float)1280 / (float)720 };
		Ref<Texture2D> m_DiffuseTexture;
		Ref<Texture2D> m_SpecularTexture;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_PlayButtonTexture;
		Ref<Texture2D> m_StopButtonTexture;
		Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		
		Ref<Scene> m_Scene;
		Entity m_TestEntity;
		Entity m_MainCamera;

		bool m_PrimaryCamera = true;
		bool m_RunScene = false;

		Ref<Model> m_Model;
		Ref<Model> m_Grid;
	};
}