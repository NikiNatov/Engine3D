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
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		E3D_CORE_LOG_INFO(event);

		dispatcher.Dispatch<WindowClosedEvent>(E3D_BIND_EVENT_FN(Application::OnWindowClosed));
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_Running = false;

		return true;
	}
}
