#pragma once

#include "Engine3D\Core\Window.h"
#include "Engine3D\Events\Event.h"
#include "Engine3D\Events\ApplicationEvent.h"

#include "Engine3D\Core\LayerStack.h"
#include "Engine3D\ImGui\ImGuiLayer.h"

#include "Engine3D\Core\Config.h"
#include "Engine3D\Core\Timestep.h"

#include <memory>

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

		void Close();

		inline Window& GetWindow() const { return *m_Window; }
	private:
		bool OnWindowClosed(WindowClosedEvent& event);
		bool OnWindowResize(WindowResizedEvent& event);
	public:
		inline static Application& GetInstance() { return *s_Instance; }
	private:
		bool m_Running = true;
		bool m_Minimized = false;
		E3D::Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		float m_LastFrameTime = 0.0f;
		float m_CurrentFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	static Application* Create();
}