#include "pch.h"
#include "Application.h"

#include "Engine3D\Core\Log.h"

#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\KeyCodes.h"

#include <GLFW\glfw3.h>

namespace E3D {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		E3D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(E3D_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{	
		while (m_Running)
		{
			Timestep ts;
			m_CurrentFrameTime = (float)glfwGetTime();
			ts = m_CurrentFrameTime - m_LastFrameTime;
			m_LastFrameTime = m_CurrentFrameTime;

			if (Input::IsKeyPressed(E3D_KEY_ESCAPE))
			{
				m_Running = false;
				break;
			}

			m_Window->OnUpdate();

			for (auto layer : m_LayerStack)
				layer->OnUpdate(ts);

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

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
		layer->OnAttach();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
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
