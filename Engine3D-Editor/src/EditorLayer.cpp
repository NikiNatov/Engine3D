#include "EditorLayer.h"

#include <ImGui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>

namespace E3D
{
	EditorLayer::EditorLayer()
		: Layer("Editor Layer")
	{
		
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_CubeTransform = glm::translate(glm::mat4(1.0f), m_CubePosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), m_CubeScale);

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		m_Framebuffer->Bind();
		Renderer::BeginScene(m_CameraController.GetCamera());

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::ClearScreen();

		m_ExampleTexture->Bind(0);
		auto shader = m_ShaderLibrary.Get("FlatColorShader");
		auto& color = m_CubeEntity.GetComponent<ColorComponent>().Color;
		shader->SetFloat3("u_Color", color);
		Renderer::Submit(m_CubeVertexArray, shader, m_CubeTransform);

		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGui::Begin("Transform");
		auto& name = m_CubeEntity.GetComponent<NameComponent>().Name;
		ImGui::Text("ID: %s", name.c_str());
		ImGui::DragFloat3("Translation", &m_CubePosition.x, 0.5f, -10.0f, 10.0f);
		ImGui::DragFloat3("Rotation", &m_CubeRotation.x, 0.5f, -180.0f, 180.0f);
		ImGui::DragFloat3("Scale", &m_CubeScale.x, 0.5f, 0.5f, 3.0f);

		auto& color = m_CubeEntity.GetComponent<ColorComponent>().Color;
		ImGui::ColorEdit3("Color", &color.x);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");

		if (m_ViewportFocused != ImGui::IsWindowFocused() && m_ViewportFocused == true)
			m_CameraController.SetViewportFocus(false);

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*) & panelSize))
		{
			m_ViewportSize = { panelSize.x, panelSize.y };
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
		}
		uint32_t texture = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)texture, { m_ViewportSize.x , m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnAttach()
	{
		auto posColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");
		posColorShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());

		m_ExampleTexture = Texture2D::Create("assets/textures/container.jpg");

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
		textureShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());
		textureShader->SetInt("u_Texture", 0);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		float cubeVertices[] =
		{
			-0.5f,  0.5f, -3.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -3.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -3.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -3.5f,   1.0f, 1.0f,

			-0.5f,  0.5f, -2.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -2.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -2.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -2.5f,   1.0f, 1.0f
		};

		uint32_t cubeIndices[] = {
				0, 2, 1,
				0, 3, 2,

				1, 2, 6,
				6, 5, 1,

				4, 5, 6,
				6, 7, 4,

				2, 3, 6,
				6, 3, 7,

				0, 7, 3,
				0, 4, 7,

				0, 1, 5,
				0, 5, 4
		};

		BufferLayout layout = {
			{ "a_Position", DataType::Float3, false },
			{ "a_TexCoord", DataType::Float2, false }
		};

		m_CubeVertexArray = VertexArray::Create();
		m_CubeVertexArray->Bind();

		m_CubeVertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		m_CubeVertexBuffer->SetLayout(layout);

		m_CubeIndexBuffer = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));

		m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);
		m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);

		m_Scene = CreateRef<Scene>();

		m_CubeEntity = m_Scene->CreateEntity("Cube Entity");
		m_CubeEntity.AddComponent<ColorComponent>(glm::vec3{ 0.8f, 0.3f, 0.2f });
	}

	void EditorLayer::OnDetach()
	{
	}
}