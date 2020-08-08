#include "pch.h"
#include "Application.h"

#include "Engine3D\Core\Log.h"

#include "Engine3D\Renderer\Renderer.h"

#include <GLFW\glfw3.h>

namespace E3D {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		E3D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = std::unique_ptr<Window>(Window::Create(name));
		m_Window->SetEventCallback(E3D_BIND_EVENT_FN(Application::OnEvent));
		Renderer::Init();

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

			if (!m_Minimized)
			{
				for (auto layer : m_LayerStack)
					layer->OnUpdate(ts);
			}

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowClosedEvent>(E3D_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizedEvent>(E3D_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (event.Handled == true)
				break;

			(*--it)->OnEvent(event);
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

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		Close();

		return true;
	}
	bool Application::OnWindowResize(WindowResizedEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::WindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}
}
