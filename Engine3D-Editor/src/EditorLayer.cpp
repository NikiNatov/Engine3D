#include "EditorLayer.h"

#include <ImGui\imgui.h>
#include <ImGuizmo\include\ImGuizmo.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "../assets/scripts/Player.h"

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

		switch (m_Scene->GetSceneState())
		{
		case Scene::SceneState::Edit:
		{
			if (m_ViewportFocused)
				m_Scene->GetCamera().Update(ts);

			m_Scene->OnEditRender();
			break;
		}
		case Scene::SceneState::Running:
			if (m_ViewportFocused)
				m_Scene->OnUpdate(ts);
			m_Scene->OnRunningRender();
			break;
		}

		m_Grid->Draw(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)));
		Renderer::Submit(m_Skybox);

		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (m_Scene->GetSceneState() != Scene::SceneState::Running && !ImGuizmo::IsUsing())
			m_Scene->GetCamera().OnEvent(event);
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
		
		m_SceneGraphPanel.OnImGuiRender();
		m_SelectedEntity = m_SceneGraphPanel.GetSelectedEntity();
		m_InspectorPanel.OnImGuiRender(m_SelectedEntity);

		m_ModelPanel.OnImGuiRender(m_SelectedEntity);

		ImGui::Begin("Environment");
		ImGui::DragFloat("Exposure", &m_Skybox->GetExposure(), 0.01f, 0.01f, 3.0f);
		ImGui::DragFloat("LOD", &m_Skybox->GetLOD(), 0.1f, 0.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Scene Control", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		if (ImGui::ImageButton((void*)m_PlayButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
		{
			if (m_Scene->GetSceneState() != Scene::SceneState::Running)
			{
				m_Scene->SetSceneState(Scene::SceneState::Running);
				m_Scene->OnSceneStart();
			}
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)m_StopButtonTexture->GetTextureID(), { 40.0f, 40.0f }, { 0, 1 }, { 1, 0 }))
		{
			if (m_Scene->GetSceneState() != Scene::SceneState::Edit)
			{
				m_Scene->SetSceneState(Scene::SceneState::Edit);
				m_Scene->OnSceneEnd();
			}
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene");
		
		auto& viewMatrix = m_Scene->GetCamera().GetViewMatrix();
		auto& projMatrix = m_Scene->GetCamera().GetProjection();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		if (m_SelectedEntity && m_Scene->GetSceneState() == Scene::SceneState::Edit)
		{
			static ImGuizmo::OPERATION currentGuizmoOp = ImGuizmo::OPERATION::TRANSLATE;
			float snap[3] = { 1.0f, 1.0f, 1.0f };
			bool useSnap = false;

			auto& entityTransform = m_SelectedEntity.GetComponent<TransformComponent>().Transform;
			glm::mat4 entityWorldTransform = entityTransform;
			glm::mat4 parentWorldTransform = glm::mat4(1.0f);

			if (m_SelectedEntity.GetComponent<SceneNodeComponent>().Parent)
			{
				auto currentParent = m_SelectedEntity.GetComponent<SceneNodeComponent>().Parent;
				while (currentParent)
				{
					auto& parentTransform = currentParent.GetComponent<TransformComponent>().Transform;
					entityWorldTransform = parentTransform * entityWorldTransform;
					currentParent = currentParent.GetComponent<SceneNodeComponent>().Parent;
				}

				parentWorldTransform = entityWorldTransform * glm::inverse(entityTransform);
			}

			if (Input::IsKeyPressed(E3D_KEY_T))
				currentGuizmoOp = ImGuizmo::TRANSLATE;
			if (Input::IsKeyPressed(E3D_KEY_R))
				currentGuizmoOp = ImGuizmo::ROTATE;
			if (Input::IsKeyPressed(E3D_KEY_E))
				currentGuizmoOp = ImGuizmo::SCALE;
			if (Input::IsKeyPressed(E3D_KEY_LEFT_SHIFT))
				useSnap = true;

			
			ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), currentGuizmoOp, ImGuizmo::LOCAL, glm::value_ptr(entityWorldTransform), NULL, useSnap ? &snap[0] : NULL);

			entityTransform = glm::inverse(parentWorldTransform) * entityWorldTransform;
		}

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&panelSize))
		{
			m_ViewportSize = { panelSize.x, panelSize.y };
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->GetCamera().SetProjection(65.0f, m_ViewportSize.x / m_ViewportSize.y);
		}
		ImGuizmo::SetDrawlist();
		uint32_t texture = m_Framebuffer->GetColorAttachment()->GetTextureID();
		ImGui::Image((void*)texture, { m_ViewportSize.x , m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		ImGui::PopStyleVar();

		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnAttach()
	{
		m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");

		m_PlayButtonTexture = Texture2D::Create("assets/textures/playButton.png");
		m_StopButtonTexture = Texture2D::Create("assets/textures/stopButton.png");

		m_Grid = CreateRef<Model>("assets/models/grid/grid.fbx");

		auto shader = m_ShaderLibrary.Load("assets/shaders/StaticModelShader.glsl");

		m_Gold = CreateRef<Material>(shader);
		m_Grass = CreateRef<Material>(shader);
		m_Marble = CreateRef<Material>(shader);
		m_Rock = CreateRef<Material>(shader);
		m_RustedIron = CreateRef<Material>(shader);
		m_Wood = CreateRef<Material>(shader);
		m_PistolMaterial = CreateRef<Material>(shader);

		// Gold
		m_Gold->SetAlbedoMap(Texture2D::Create("assets/PBR/gold/lightgold_albedo.png", {}, true));
		m_Gold->SetNormalMap(Texture2D::Create("assets/PBR/gold/lightgold_normal-ogl.png", {}, true));
		m_Gold->SetMetalnessMap(Texture2D::Create("assets/PBR/gold/lightgold_metallic.png", {}, true));
		m_Gold->SetRoughnessMap(Texture2D::Create("assets/PBR/gold/lightgold_roughness.png", {}, true));


		// Grass
		m_Grass->SetAlbedoMap(Texture2D::Create("assets/PBR/grass/leafy-grass2-albedo.png", {}, true));
		m_Grass->SetNormalMap(Texture2D::Create("assets/PBR/grass/leafy-grass2-normal-ogl.png", {}, true));
		m_Grass->SetMetalnessMap(Texture2D::Create("assets/PBR/grass/leafy-grass2-metallic.png", {}, true));
		m_Grass->SetRoughnessMap(Texture2D::Create("assets/PBR/grass/leafy-grass2-roughness.png", {}, true));

		// Marble
		m_Marble->SetAlbedoMap(Texture2D::Create("assets/PBR/marble/stringy_marble_albedo.png", {}, true));
		m_Marble->SetNormalMap(Texture2D::Create("assets/PBR/marble/stringy_marble_Normal-ogl.png", {}, true));
		m_Marble->SetMetalnessMap(Texture2D::Create("assets/PBR/marble/stringy_marble_Metallic.png", {}, true));
		m_Marble->SetRoughnessMap(Texture2D::Create("assets/PBR/marble/stringy_marble_Roughness.png", {}, true));

		// Rock
		m_Rock->SetAlbedoMap(Texture2D::Create("assets/PBR/rock/layered-rock1-albedo.png", {}, true));
		m_Rock->SetNormalMap(Texture2D::Create("assets/PBR/rock/layered-rock1-normal-ogl.png", {}, true));
		m_Rock->SetMetalnessMap(Texture2D::Create("assets/PBR/rock/layered-rock1-Metalness.png", {}, true));
		m_Rock->SetRoughnessMap(Texture2D::Create("assets/PBR/rock/layered-rock1-rough.png", {}, true));

		// Rusted Iron
		m_RustedIron->SetAlbedoMap(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_basecolor.png", {}, true));
		m_RustedIron->SetNormalMap(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_normal.png", {}, true));
		m_RustedIron->SetMetalnessMap(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_metallic.png", {}, true));
		m_RustedIron->SetRoughnessMap(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_roughness.png", {}, true));

		// Wood
		m_Wood->SetAlbedoMap(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-albedo.png", {}, true));
		m_Wood->SetNormalMap(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-normal.png", {}, true));
		m_Wood->SetMetalnessMap(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-metal.png", {}, true));
		m_Wood->SetRoughnessMap(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-roughness.png", {}, true));

		// Pistol
		m_PistolMaterial->SetAlbedoMap(Texture2D::Create("assets/models/gun/Textures/Cerberus_A.tga", {}, true));
		m_PistolMaterial->SetNormalMap(Texture2D::Create("assets/models/gun/Textures/Cerberus_N.tga", {}, true));
		m_PistolMaterial->SetMetalnessMap(Texture2D::Create("assets/models/gun/Textures/Cerberus_M.tga", {}, true));
		m_PistolMaterial->SetRoughnessMap(Texture2D::Create("assets/models/gun/Textures/Cerberus_R.tga", {}, true));

		m_Framebuffer = Framebuffer::Create({1280, 720, TextureTarget::Texture2D, TextureFormat::RGBA8, 1});

		m_Skybox = CreateRef<Skybox>();
		m_Skybox->SetTexture("assets/textures/skybox/the_sky_is_on_fire_4k.hdr");

		m_Scene = CreateRef<Scene>(m_Skybox);

		m_Pistol = m_Scene->CreateEntity("Pistol");
		m_Pistol.AddComponent<MeshComponent>("assets/models/gun/Cerberus_LP.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_PistolMaterial);
		m_Pistol.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f));
		m_Pistol.AddComponent<ScriptComponent>().Bind<Player>();

		m_Plane = m_Scene->CreateEntity("Plane");
		m_Plane.AddComponent<MeshComponent>("assets/models/primitives/plane.fbx");
		m_Plane.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 11.0f, 0.0f));

		m_Cube = m_Scene->CreateEntity("Cube");
		m_Cube.AddComponent<MeshComponent>("assets/models/primitives/cube.fbx");
		m_Cube.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(26.0f, 19.0f, 8.0f));

		m_Cone = m_Scene->CreateEntity("Cone");
		m_Cone.AddComponent<MeshComponent>("assets/models/primitives/cone.fbx");
		m_Cone.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f, 19.0f, 8.0f));

		m_MainCamera = m_Scene->CreateEntity("Main Camera");
		m_MainCamera.AddComponent<MeshComponent>("assets/models/camera/camera.obj");
		m_MainCamera.AddComponent<CameraComponent>().Primary = true;
		auto& cameraTransform = m_MainCamera.GetComponent<TransformComponent>().Transform;
		cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 35.0f));

		auto model = CreateRef<Model>("assets/models/primitives/globe-sphere.fbx");

		Entity e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_Gold);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_Grass);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_Marble);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_Rock);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_RustedIron);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>("assets/models/primitives/globe-sphere.fbx").Mesh->GetMeshes()[0]->SetMaterial(m_Wood);
		e.GetComponent<TransformComponent>().Transform = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 8.0f, -90.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));

		auto& planeNC = m_Plane.GetComponent<SceneNodeComponent>();
		auto& cubeNC = m_Cube.GetComponent<SceneNodeComponent>();
		auto& coneNC = m_Cone.GetComponent<SceneNodeComponent>();
		//auto& sphereNC = m_Sphere.GetComponent<SceneNodeComponent>();
		planeNC.FirstChild = m_Cube;
		cubeNC.NextSibling = m_Cone;
		coneNC.PreviousSibling = m_Cube;
		coneNC.NextSibling = m_Sphere;

		cubeNC.Parent = m_Plane;
		coneNC.Parent = m_Plane;
		//sphereNC.Parent = m_Plane;

		/*m_Vader = m_Scene->CreateEntity("Darth Vader");
		m_Vader.AddComponent<MeshComponent>("assets/models/starwars/darth vader/vaderModified.fbx");
		auto& transform = m_Vader.GetComponent<TransformComponent>().Transform;
		

		m_MainCamera = m_Scene->CreateEntity("Main Camera");
		m_MainCamera.AddComponent<MeshComponent>("assets/models/camera/camera.obj");
		m_MainCamera.AddComponent<CameraComponent>().Primary = true;
		auto& cameraTransform = m_MainCamera.GetComponent<TransformComponent>().Transform;
		cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 35.0f));


		m_SpaceShip = m_Scene->CreateEntity("Tie-Fighter");
		m_SpaceShip.AddComponent<MeshComponent>("assets/models/starwars/spaceship/tie-fighter.fbx");
		auto& transform2 = m_SpaceShip.GetComponent<TransformComponent>().Transform;
		transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f));
		m_SpaceShip.AddComponent<ScriptComponent>().Bind<Player>();
		

		m_Terrain = m_Scene->CreateEntity("Terrain");
		m_Terrain.AddComponent<MeshComponent>("assets/models/terrain/lavaTerrain.fbx");

		m_SpaceShip.GetComponent<SceneNodeComponent>().FirstChild = m_Vader;
		m_Vader.GetComponent<SceneNodeComponent>().Parent = m_SpaceShip;*/

		m_SceneGraphPanel.SetScene(m_Scene);
	}

	void EditorLayer::OnDetach()
	{
	}
}