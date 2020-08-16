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

		if(m_RunScene)
			m_Scene->OnUpdate(ts);
		else
		{
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			Renderer::BeginScene(m_CameraController.GetCamera());
			auto& transform = m_TestEntity.GetComponent<TransformComponent>().Transform;
			m_Model->Draw(transform);

			auto& mesh = m_MainCamera.GetComponent<MeshComponent>().Mesh;
			auto& cameraTransform = m_MainCamera.GetComponent<TransformComponent>().Transform;
			mesh->Draw(cameraTransform);
		}

		m_Grid->Draw(glm::mat4(1.0f));

		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if(!m_RunScene && !ImGuizmo::IsUsing())
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

		ImGui::Begin("Model");
		int nodeID1 = 0;
		static int selectedNodeID1 = 0;
		m_Model->GetRootNode()->RenderTree(nodeID1, selectedNodeID1);

		auto& mesh = m_MainCamera.GetComponent<MeshComponent>().Mesh;
		mesh->GetRootNode()->RenderTree(nodeID1, selectedNodeID1);
		ImGui::End();

		ImGui::Begin("Properties");
	
		if (m_TestEntity)
		{
			ImGui::Separator();
			auto& name = m_TestEntity.GetComponent<NameComponent>().Name;
			auto& transform = m_TestEntity.GetComponent<TransformComponent>().Transform;
			ImGui::InputText("Tag##TestEntity", (char*)name.c_str(), 25);
			ImGui::Separator();


			glm::vec3 position, rotation, scale;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));
			
			if (ImGui::CollapsingHeader("Transform##TestEntity"))
			{
				ImGui::DragFloat3("Translation##TestEntity", &position.x, 0.5f, -10.0f, 10.0f);
				ImGui::DragFloat3("Rotation##TestEntity", &rotation.x, 0.5f, -180.0f, 180.0f);
				ImGui::DragFloat3("Scale##TestEntity", &scale.x, 0.5f, 0.5f, 3.0f);
			}
			ImGui::Separator();

			ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
		}

		if (m_MainCamera)
		{
			ImGui::Separator();
			auto& name = m_MainCamera.GetComponent<NameComponent>().Name;
			auto& transform = m_MainCamera.GetComponent<TransformComponent>().Transform;
			ImGui::InputText("Tag##CameraEntity", (char*)name.c_str(), 25);
			ImGui::Separator();


			glm::vec3 position, rotation, scale;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));

			if (ImGui::CollapsingHeader("Transform##Camera"))
			{
				ImGui::DragFloat3("Translation##Camera", &position.x, 0.5f, -100.0f, 100.0f);
				ImGui::DragFloat3("Rotation##Camera", &rotation.x, 0.5f, -180.0f, 180.0f);
				ImGui::DragFloat3("Scale##Camera", &scale.x, 0.5f, 0.5f, 3.0f);
			}
			ImGui::Separator();

			ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));
		}
		ImGui::Separator();

		ImGui::End();
		ImGui::Begin("Materials");
		ImGui::Text("Diffuse Texture");
		ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		ImGui::Text("Specular Texture");
		ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		/*ImGui::Separator();
		static bool diffuseTex = false;
		static bool specularTex = false;
		ImGui::Text("Diffuse Texture");
		if (m_RubyMaterial->HasDiffuseTexture())
		{
			ImGui::Image((void*)m_RubyMaterial->GetDiffuseTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Checkbox("Use##DiffuseTexture", &diffuseTex))
				m_RubyMaterial->UseDiffuseTexture(diffuseTex);
			ImGui::SameLine();
			ImGui::ColorEdit3("", &m_RubyMaterial->GetDiffuseColor().x, ImGuiColorEditFlags_NoInputs);
			ImGui::EndGroup();
		}
		else
			ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

		ImGui::Text("Specular Texture");
		if (m_RubyMaterial->HasSpecularTexture())
		{
			ImGui::Image((void*)m_RubyMaterial->GetSpecularTexture()->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Checkbox("Use##SpecularTexture", &specularTex))
				m_RubyMaterial->UseSpecularTexture(true);

			ImGui::SameLine();
			ImGui::ColorEdit3("", &m_RubyMaterial->GetSpecularColor().x, ImGuiColorEditFlags_NoInputs);
			ImGui::EndGroup();
		}
		else
			ImGui::Image((void*)m_CheckerboardTexture->GetTextureID(), ImVec2{ 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
		
		ImGui::DragFloat("Shininess", &m_RubyMaterial->GetShininess(), 0.5f, 0.0f, 180.0f);
		ImGui::DragFloat("Transparency", &m_RubyMaterial->GetTransparency(), 0.03f, 0.0f, 1.0f);*/

		ImGui::Separator();
		static bool gizmoEnabled = true;
		ImGui::Checkbox("Gizmos Toggle", &gizmoEnabled);

		static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;

		if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE))
			operation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE))
			operation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE))
			operation = ImGuizmo::SCALE;

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene");
		
		if (gizmoEnabled && !m_RunScene)
		{
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)ImGui::GetWindowWidth(), (float)ImGui::GetWindowHeight());

			ImGuizmo::SetID(0);
			auto& transform = m_TestEntity.GetComponent<TransformComponent>().Transform;
			
			ImGuizmo::Manipulate(glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()), glm::value_ptr(m_CameraController.GetCamera().GetProjection()), operation, ImGuizmo::LOCAL, glm::value_ptr(transform));
		}


		if (ImGui::ImageButton((void*)m_PlayButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
			m_RunScene = true;
		ImGui::SameLine();
		if(ImGui::ImageButton((void*)m_StopButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
			m_RunScene = false;

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

		auto materialShader = m_ShaderLibrary.Load("assets/shaders/MaterialShader.glsl");

		m_Model = CreateRef<Model>("assets/models/darth vader/darthVader.fbx");
		m_Grid = CreateRef<Model>("assets/models/grid/grid.fbx");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_Scene = CreateRef<Scene>();

		m_TestEntity = m_Scene->CreateEntity("Test Entity");
		m_TestEntity.AddComponent<MeshComponent>(m_Model);

		auto& transform = m_TestEntity.GetComponent<TransformComponent>().Transform;
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

		m_MainCamera = m_Scene->CreateEntity("Main Camera");
		m_MainCamera.AddComponent<MeshComponent>("assets/models/camera/camera.obj");
		m_MainCamera.AddComponent<CameraComponent>().Primary = true;
		auto& cameraTransform = m_MainCamera.GetComponent<TransformComponent>().Transform;
		cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 35.0f));
	}

	void EditorLayer::OnDetach()
	{
	}
}