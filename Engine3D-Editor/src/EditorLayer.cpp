#include "EditorLayer.h"

#include <ImGui\imgui.h>
#include <ImGuizmo\include\ImGuizmo.h>
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
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::ClearScreen();

		m_Grid->Draw(glm::mat4(1.0f));

		switch (m_Scene->GetSceneState())
		{
		case Scene::SceneState::Edit:
		{
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			Renderer::BeginScene(m_CameraController.GetCamera());
			
			m_Scene->OnEditRender();
			break;
		}
		case Scene::SceneState::Running:
			m_Scene->OnRunningRender();
			break;
		}

		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if(m_Scene->GetSceneState() != Scene::SceneState::Running && !ImGuizmo::IsUsing())
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

		m_Scene->GetSceneGraph().DisplaySceneHierarchy();

		
		ImGui::Begin("Materials");
		ImGui::Text("Diffuse Texture");
		ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		ImGui::Text("Specular Texture");
		ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		
		ImGui::Separator();
		static bool gizmoEnabled = true;
		ImGui::Checkbox("Gizmos Toggle", &gizmoEnabled);

		static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
		static ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;

		if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE))
			operation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE))
			operation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE))
			operation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL))
			mode = ImGuizmo::MODE::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD))
			mode = ImGuizmo::MODE::WORLD;

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene");
		
		/*if (gizmoEnabled && m_Scene->GetSceneState() != Scene::SceneState::Running)
		{
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)ImGui::GetWindowWidth(), (float)ImGui::GetWindowHeight());

			auto& transform1 = m_TestEntity->GetComponent<TransformComponent>().Transform;
			auto& transform2 = m_MainCamera->GetComponent<TransformComponent>().Transform;
			auto& transform3 = m_AnotherEntity->GetComponent<TransformComponent>().Transform;
			ImGuizmo::SetID(0);
			
			ImGuizmo::Manipulate(glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()), glm::value_ptr(m_CameraController.GetCamera().GetProjection()), operation, mode, glm::value_ptr(transform1));

			ImGuizmo::SetID(1);

			ImGuizmo::Manipulate(glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()), glm::value_ptr(m_CameraController.GetCamera().GetProjection()), operation, mode, glm::value_ptr(transform2));

			ImGuizmo::SetID(2);

			ImGuizmo::Manipulate(glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()), glm::value_ptr(m_CameraController.GetCamera().GetProjection()), operation, mode, glm::value_ptr(transform3));
		}*/


		if (ImGui::ImageButton((void*)m_PlayButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
			m_Scene->SetSceneState(Scene::SceneState::Running);
		ImGui::SameLine();
		if(ImGui::ImageButton((void*)m_StopButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
			m_Scene->SetSceneState(Scene::SceneState::Edit);

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
		ImGuizmo::SetDrawlist();
		uint32_t texture = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)texture, { m_ViewportSize.x , m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnAttach()
	{
		m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");

		m_DiffuseTexture = Texture2D::Create("assets/textures/rubyTexture.jpg");
		m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");
		m_SpecularTexture = Texture2D::Create("assets/textures/specular.png");
		m_PlayButtonTexture = Texture2D::Create("assets/textures/playButton.png");
		m_StopButtonTexture = Texture2D::Create("assets/textures/stopButton.png");

		//auto materialShader = m_ShaderLibrary.Load("assets/shaders/MaterialShader.glsl");

		m_Model = CreateRef<Model>("assets/models/starwars/darth vader/darthVader.fbx");
		m_Grid = CreateRef<Model>("assets/models/grid/grid.fbx");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_Scene = CreateRef<Scene>();

		
	}

	void EditorLayer::OnDetach()
	{
	}
}