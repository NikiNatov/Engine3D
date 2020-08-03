#include "pch.h"
#include "Application.h"

#include "Engine3D\Config.h"
#include "Engine3D\Log.h"

#include <glad\glad.h>

namespace E3D {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		E3D_CORE_ASSERT(s_Instance, "Application is null!");

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(E3D_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{	

		while (m_Running)
		{
			OnUpdate();
			
		}
	}
	void Application::OnUpdate()
	{
		m_Window->OnUpdate();

		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto layer : m_LayerStack)
			layer->OnUpdate();
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		E3D_CORE_LOG_INFO(event);

		dispatcher.Dispatch<WindowClosedEvent>(E3D_BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);

			if (event.Handled == true)
				break;
		}
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
	}

	void Application::PopOverlay(Layer* layer)
	{
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_Running = false;

		return true;
	}
}
