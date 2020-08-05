#include "pch.h"
#include "Application.h"

#include "Engine3D\Core\Log.h"

#include <glad\glad.h>

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

		// Temporary //////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_PositionColorShader = E3D::Shader::Create("src/assets/shaders/PositionColorShader.glsl");

		float triangle[] = {
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f
		};

		float quad[] = {
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	 0.5f, 0.5f, 0.0f
		};

		uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };

		BufferLayout layout = {
			{ "a_Position", DataType::Float3, false },
			{ "a_Color", DataType::Float3, false }
		};

		m_QuadVertexArray = VertexArray::Create();
		m_QuadVertexArray->Bind();

		m_QuadVertexBuffer = VertexBuffer::Create(quad, sizeof(quad));
		m_QuadVertexBuffer->SetLayout(layout);

		m_QuadIndexBuffer = IndexBuffer::Create(quadIndices, 6);

		m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);
		m_QuadVertexArray->SetIndexBuffer(m_QuadIndexBuffer);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{	
		while (m_Running)
		{
			m_Window->OnUpdate();

			// Temporary //////////////////////////////////////////////////////////////////////////////////////////////////////////

			glClearColor(0.2, 0.2, 0.2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_PositionColorShader->Bind();
			m_QuadVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_QuadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (auto layer : m_LayerStack)
				layer->OnUpdate();

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
