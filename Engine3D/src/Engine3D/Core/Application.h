#pragma once

#include "Engine3D\Core\Window.h"
#include "Engine3D\Events\Event.h"
#include "Engine3D\Events\ApplicationEvent.h"

#include "Engine3D\Core\LayerStack.h"
#include "Engine3D\ImGui\ImGuiLayer.h"

#include <Engine3D\Core\Config.h>

#include <memory>

// Temporary
#include "Engine3D\Renderer\Shader.h"
#include "Engine3D\Renderer\Buffer.h"
#include "Engine3D\Renderer\VertexArray.h"

namespace E3D {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushOverlay(Layer* layer);
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		inline Window& GetWindow() const { return *m_Window; }
	private:
		bool OnWindowClosed(WindowClosedEvent& event);
	public:
		inline static Application& GetInstance() { return *s_Instance; }
	private:
		bool m_Running = true;

		E3D::Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		// To be moved in TestGame
		E3D::Ref<E3D::Shader> m_PositionColorShader;
		E3D::Ref<E3D::VertexBuffer> m_QuadVertexBuffer;
		E3D::Ref<E3D::IndexBuffer> m_QuadIndexBuffer;
		E3D::Ref<E3D::VertexArray> m_QuadVertexArray;
	private:
		static Application* s_Instance;
	};

	static Application* Create();
}