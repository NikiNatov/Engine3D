#include "EditorLayer.h"

#include <ImGui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

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


		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::ClearScreen();

		if(m_RunScene)
			m_Scene->OnUpdate(ts);
		else
		{
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			Renderer::BeginScene(m_CameraController.GetCamera());
		}

		Renderer::Submit(m_CubeVertexArray, m_RubyMaterial, m_CubeTransform);

		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if(!m_RunScene)
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


		ImGui::Begin("Properties");
		if (m_CubeEntity)
		{
			ImGui::Separator();
			auto& name = m_CubeEntity.GetComponent<NameComponent>().Name;
			ImGui::Text("ID: %s", name.c_str());
			ImGui::Separator();
			ImGui::DragFloat3("Translation", &m_CubePosition.x, 0.5f, -10.0f, 10.0f);
			ImGui::DragFloat3("Rotation", &m_CubeRotation.x, 0.5f, -180.0f, 180.0f);
			ImGui::DragFloat3("Scale", &m_CubeScale.x, 0.5f, 0.5f, 3.0f);
			ImGui::Separator();
		}

		if (ImGui::Checkbox("Main Camera", &m_PrimaryCamera))
		{
			m_MainCamera.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		ImGui::Separator();
		ImGui::DragFloat3("Main", glm::value_ptr(m_MainCamera.GetComponent<TransformComponent>().Transform[3]));
		ImGui::DragFloat3("Secondary", glm::value_ptr(m_SecondCamera.GetComponent<TransformComponent>().Transform[3]));
		ImGui::Separator();

		ImGui::End();

		ImGui::Begin("Materials");
		ImGui::DragFloat3("Ambient", &m_RubyMaterial->GetAmbientColor().x, 0.03f, 0.0f, 1.0f);
		ImGui::DragFloat3("Diffuse", &m_RubyMaterial->GetDiffuseColor().x, 0.03f, 0.0f, 1.0f);
		ImGui::DragFloat3("Specular", &m_RubyMaterial->GetSpecularColor().x, 0.03f, 0.0f, 1.0f);
		ImGui::DragFloat("Shininess", &m_RubyMaterial->GetShininess(), 0.5f, 0.0f, 180.0f);
		ImGui::DragFloat("Transparency", &m_RubyMaterial->GetTransparency(), 0.03f, 0.0f, 1.0f);
		ImGui::Separator();
		static bool diffuseTex = false;
		static bool specularTex = false;
		ImGui::Text("Diffuse Texture");
		if (m_RubyMaterial->HasDiffuseTexture())
		{
			ImGui::Image((void*)m_RubyMaterial->GetDiffuseTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			if (ImGui::Checkbox("Use##DiffuseTexture", &diffuseTex))
				m_RubyMaterial->UseDiffuseTexture(diffuseTex);
		}
		else
			ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

		ImGui::Text("Specular Texture");
		if (m_RubyMaterial->HasSpecularTexture())
		{
			ImGui::Image((void*)m_RubyMaterial->GetSpecularTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			if (ImGui::Checkbox("Use##SpecularTexture", &specularTex))
				m_RubyMaterial->UseSpecularTexture(specularTex);
		}
		else
			ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		
		ImGui::Separator();

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene");
		ImGui::Checkbox("Run Scene", &m_RunScene);

		if (m_ViewportFocused != ImGui::IsWindowFocused() && m_ViewportFocused == true)
			m_CameraController.SetViewportFocus(false);

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&panelSize))
		{
			m_ViewportSize = { panelSize.x, panelSize.y };
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
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
		m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");

		m_DiffuseTexture = Texture2D::Create("assets/textures/diffuse.png");
		m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");
		m_SpecularTexture = Texture2D::Create("assets/textures/specular.png");

		auto materialShader = m_ShaderLibrary.Load("assets/shaders/MaterialShader.glsl");

		m_RubyMaterial = CreateRef<Material>(materialShader);
		m_RubyMaterial->SetAmbientColor({ 0.1745f, 0.01175f, 0.01175f });
		m_RubyMaterial->SetDiffuseColor({ 0.61424f, 0.04136f, 0.04136f });
		m_RubyMaterial->SetSpecularColor({ 0.727811f, 0.626959f, 0.626959f });
		m_RubyMaterial->SetShininess(76.8f);
		m_RubyMaterial->SetTransparency(1.0f);
		m_RubyMaterial->SetDiffuseTexture(m_DiffuseTexture);
		m_RubyMaterial->SetSpecularTexture(m_SpecularTexture);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		float cubeVertices[] =
		{
			-0.5f,  0.5f, -3.5f,   0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -3.5f,   0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -3.5f,   1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -3.5f,   1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
											      
			-0.5f,  0.5f, -2.5f,   0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -2.5f,   0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -2.5f,   1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -2.5f,   1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
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
			{ "a_TexCoord", DataType::Float2, false },
			{ "a_Normal", DataType::Float3, false },
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

		m_MainCamera = m_Scene->CreateEntity("Main Camera");
		m_MainCamera.AddComponent<CameraComponent>().Primary = true;

		m_SecondCamera = m_Scene->CreateEntity("Second Camera");
		auto& cameraComponent = m_SecondCamera.AddComponent<CameraComponent>();
		cameraComponent.Primary = false;
	}

	void EditorLayer::OnDetach()
	{
	}
}