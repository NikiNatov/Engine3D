#include "EditorLayer.h"

#include <ImGui\imgui.h>
#include <ImGuizmo\include\ImGuizmo.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Engine3D\Scene\SceneSerializer.h"
#include "Engine3D\ResourceManager\MeshManager.h"

#include "../assets/scripts/Player.h"

#include <fstream>

namespace E3D
{
	EditorLayer::EditorLayer()
		: Layer("Editor Layer")
	{
		
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		RenderCommand::ClearScreen();

		switch (m_Scene->GetSceneState())
		{
		case Scene::SceneState::Edit:
		{
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

		Renderer::SetRenderMode(RenderMode::Lines);
		Renderer::Submit(m_Grid, glm::mat4(1.0f));
		Renderer::SetRenderMode(RenderMode::Triangles);


		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (m_Scene->GetSceneState() != Scene::SceneState::Running && !ImGuizmo::IsUsing())
			m_Scene->GetCamera().OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(E3D_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneGraphPanel.OnImGuiRender();
		m_SelectedEntity = m_SceneGraphPanel.GetSelectedEntity();
		m_InspectorPanel.OnImGuiRender(m_SelectedEntity);

		m_MaterialPanel.OnImGuiRender(m_SelectedEntity);
		m_AssetsPanel.OnImGuiRender();

		

		ImGui::Begin("Environment");
		if (ImGui::Button("Upload Environment Map"))
		{
			const std::string& path = FileDialog::OpenTextureFile();

			if(!path.empty())
				m_Skybox->SetTexture(path);
		}
		ImGui::DragFloat("Exposure", &m_Skybox->GetExposure(), 0.01f, 0.01f, 5.0f);
		ImGui::DragFloat("LOD", &m_Skybox->GetLOD(), 0.1f, 0.0f, 10.0f);

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene Control", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 1 });
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
		ImGui::PopStyleColor();
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Scene");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&panelSize))
		{
			m_ViewportSize = { panelSize.x, panelSize.y };
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->GetCamera().SetProjection(65.0f, m_ViewportSize.x / m_ViewportSize.y);
		}
		
		uint32_t texture = m_Framebuffer->GetColorAttachment()->GetTextureID();
		ImGui::Image((void*)texture, { m_ViewportSize.x , m_ViewportSize.y }, { 0, 1 }, { 1, 0 });


		if (m_SelectedEntity && m_Scene->GetSceneState() == Scene::SceneState::Edit && m_GuizmoOperation != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			auto& viewMatrix = m_Scene->GetCamera().GetViewMatrix();
			auto& projMatrix = m_Scene->GetCamera().GetProjection();

			float snapValue = m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

			float snap[3] = { snapValue, snapValue, snapValue };
			m_GuizmoSnap = Input::IsKeyPressed(E3D_KEY_LEFT_CONTROL);

			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			auto entityTransform = tc.GetTransform();
			glm::mat4 entityWorldTransform = entityTransform;
			glm::mat4 parentWorldTransform = glm::mat4(1.0f);

			if (m_SelectedEntity.GetComponent<SceneNodeComponent>().Parent)
			{
				auto currentParent = m_SelectedEntity.GetComponent<SceneNodeComponent>().Parent;
				while (currentParent)
				{
					auto parentTransform = currentParent.GetComponent<TransformComponent>().GetTransform();
					entityWorldTransform = parentTransform * entityWorldTransform;
					currentParent = currentParent.GetComponent<SceneNodeComponent>().Parent;
				}

				parentWorldTransform = entityWorldTransform * glm::inverse(entityTransform);
			}

			ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), (ImGuizmo::OPERATION)m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityWorldTransform), NULL, m_GuizmoSnap ? &snap[0] : NULL);

			entityTransform = glm::inverse(parentWorldTransform) * entityWorldTransform;

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(entityTransform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

				glm::vec3 deltaRotation = glm::radians(rotation) - tc.Rotation;

				tc.Translation = translation;
				//tc.Rotation = glm::radians(rotation);
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::PopStyleVar();

		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnAttach()
	{

		//ShaderLibrary::Load("assets/shaders/FlatColorShader.glsl");

		m_PlayButtonTexture = TextureManager::LoadTexture(Texture2D::Create("assets/textures/playButton.png", {}, true));
		m_StopButtonTexture = TextureManager::LoadTexture(Texture2D::Create("assets/textures/pauseButton.png", {}, true));

		//auto shader = ShaderLibrary::Load("assets/shaders/StaticModelShader.glsl");

		LoadAssets();
		auto& shader = ShaderLibrary::Get("StaticModelShader");
		MeshManager::LoadMesh(MeshFactory::CreateCube(1.0f, CreateRef<Material>(shader)));
		MeshManager::LoadMesh(MeshFactory::CreatePlane({ 1.0f, 1.0f }, CreateRef<Material>(shader)));
		m_Grid = MeshFactory::CreateGrid(80, 80, 80, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader")));

		m_Framebuffer = Framebuffer::Create({ 1280, 720, TextureTarget::Texture2D, TextureFormat::RGBA8, 1 });

		m_Skybox = CreateRef<Skybox>();
		m_Skybox->SetTexture("assets/textures/skybox/birchwood_4k.hdr");

		m_Scene = CreateRef<Scene>(m_Skybox);

		m_SceneGraphPanel.SetScene(m_Scene);
		m_Gold = CreateRef<Material>(shader);
		m_Grass = CreateRef<Material>(shader);
		m_Marble = CreateRef<Material>(shader);
		m_Rock = CreateRef<Material>(shader);
		m_RustedIron = CreateRef<Material>(shader);
		m_Wood = CreateRef<Material>(shader);
		m_PistolMaterial = CreateRef<Material>(shader);
		

		// Gold
		m_Gold->SetName("GoldMaterial");
		m_Gold->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/gold/lightgold_albedo.png", {}, true)));
		m_Gold->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/gold/lightgold_normal-ogl.png", {}, true)));
		m_Gold->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/gold/lightgold_metallic.png", {}, true)));
		m_Gold->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/gold/lightgold_roughness.png", {}, true)));

		// Grass
		m_Grass->SetName("GrassMaterial");
		m_Grass->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/grass/leafy-grass2-albedo.png", {}, true)));
		m_Grass->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/grass/leafy-grass2-normal-ogl.png", {}, true)));
		m_Grass->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/grass/leafy-grass2-metallic.png", {}, true)));
		m_Grass->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/grass/leafy-grass2-roughness.png", {}, true)));

		// Marble
		m_Marble->SetName("MarbleMaterial");
		m_Marble->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/marble/stringy_marble_albedo.png", {}, true)));
		m_Marble->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/marble/stringy_marble_Normal-ogl.png", {}, true)));
		m_Marble->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/marble/stringy_marble_Metallic.png", {}, true)));
		m_Marble->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/marble/stringy_marble_Roughness.png", {}, true)));

		// Rock
		m_Rock->SetName("RockMaterial");
		m_Rock->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rock/layered-rock1-albedo.png", {}, true)));
		m_Rock->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rock/layered-rock1-normal-ogl.png", {}, true)));
		m_Rock->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rock/layered-rock1-Metalness.png", {}, true)));
		m_Rock->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rock/layered-rock1-rough.png", {}, true)));

		// Rusted Iron
		m_RustedIron->SetName("RustedIronMaterial");
		m_RustedIron->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_basecolor.png", {}, true)));
		m_RustedIron->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_normal.png", {}, true)));
		m_RustedIron->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_metallic.png", {}, true)));
		m_RustedIron->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/rusted_iron/rustediron2_roughness.png", {}, true)));

		// Wood
		m_Wood->SetName("WoodMaterial");
		m_Wood->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-albedo.png", {}, true)));
		m_Wood->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-normal.png", {}, true)));
		m_Wood->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-metal.png", {}, true)));
		m_Wood->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/PBR/wood/bamboo-wood-semigloss-roughness.png", {}, true)));

		// Pistol
		m_PistolMaterial->SetName("PistolMaterial");
		m_PistolMaterial->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/gun/Textures/Cerberus_A.tga", {}, true)));
		m_PistolMaterial->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/gun/Textures/Cerberus_N.tga", {}, true)));
		m_PistolMaterial->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/gun/Textures/Cerberus_M.tga", {}, true)));
		m_PistolMaterial->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/gun/Textures/Cerberus_R.tga", {}, true)));

		// Pistol
		m_TestMat = CreateRef<Material>(shader);
		m_TestMat->SetName("TestMat");
		m_TestMat->SetAlbedoMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/pistol/m1911_color.png", {}, true)));
		m_TestMat->SetNormalMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/pistol/m1911_normal.png", {}, true)));
		m_TestMat->SetRoughnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/pistol/m1911_roughness.png", {}, true)));
		m_TestMat->SetMetalnessMap(TextureManager::LoadTexture(Texture2D::Create("assets/models/pistol/m1911_metalness.png", {}, true)));

		MaterialManager::LoadMaterial(m_Gold);
		MaterialManager::LoadMaterial(m_Grass);
		MaterialManager::LoadMaterial(m_Marble);
		MaterialManager::LoadMaterial(m_Rock);
		MaterialManager::LoadMaterial(m_RustedIron);
		MaterialManager::LoadMaterial(m_Wood);
		MaterialManager::LoadMaterial(m_PistolMaterial);
		MaterialManager::LoadMaterial(m_TestMat);

#if 0
		auto cameraModel = ModelManager::GetModel("assets/models/camera/camera.obj");
		auto& cameraMesh = cameraModel->GetMesh(0);
		m_MainCamera = m_Scene->CreateEntity("Main Camera");
		m_MainCamera.AddComponent<MeshComponent>(cameraMesh);
		m_MainCamera.AddComponent<CameraComponent>().Primary = true;
		m_MainCamera.GetComponent<TransformComponent>().Translation = { 0.0f, 1.0f, 5.0f };
		m_MainCamera.GetComponent<TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };

		auto pistol = ModelManager::GetModel("assets/models/gun/Cerberus_LP.fbx");

		auto spaceShip = ModelManager::GetModel("assets/models/starwars/spaceship/tie-fighter.fbx");
		m_Player = m_Scene->CreateFromModel(spaceShip, "Player");
		m_Player.AddComponent<ScriptComponent>().Bind<Player>();
		m_Player.GetComponent<TransformComponent>().Scale = { 9.0f, 9.0f, 9.0f };

		m_Player.AddChild(m_MainCamera);

		auto sphereModel = ModelManager::GetModel("assets/models/primitives/sphere.fbx");
		auto& sphereMesh = sphereModel->GetMesh(0);

		Entity e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_Gold);
		e.GetComponent<TransformComponent>().Translation = { -100.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_Grass);
		e.GetComponent<TransformComponent>().Translation = { -60.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_Marble);
		e.GetComponent<TransformComponent>().Translation = { -20.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_Rock);
		e.GetComponent<TransformComponent>().Translation = { 20.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_RustedIron);
		e.GetComponent<TransformComponent>().Translation = { 60.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };

		e = m_Scene->CreateEntity("Sphere");
		e.AddComponent<MeshComponent>(std::make_shared<Mesh>(*sphereMesh)).Mesh->SetMaterial(m_Wood);
		e.GetComponent<TransformComponent>().Translation = { 100.0f, 8.0f, -90.0f };
		e.GetComponent<TransformComponent>().Scale = { 30.0f, 30.0f, 30.0f };		
#endif
	}

	void EditorLayer::OnDetach()
	{
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{

		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(E3D_KEY_LEFT_CONTROL) || Input::IsKeyPressed(E3D_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(E3D_KEY_LEFT_SHIFT) || Input::IsKeyPressed(E3D_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
			case E3D_KEY_S:
			{
				if (control)
					SaveSceneAs();

				break;
			}
			case E3D_KEY_N:
			{
				if (control)
					NewScene();

				break;
			}
			case E3D_KEY_O:
			{
				if (control)
					OpenScene();

				break;
			}
			case E3D_KEY_Q:
				m_GuizmoOperation = -1;
				break;
			case E3D_KEY_T:
				m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case E3D_KEY_R:
				m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;
				break;
			case E3D_KEY_E:
				m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return true;
	}

	void EditorLayer::LoadAssets()
	{
		LoadShaders();
		LoadTextures();
		LoadModels();
	}

	void EditorLayer::LoadModels()
	{
		std::ifstream stream("src/Models.txt");
		std::string path;
		std::vector<std::string> filepaths;

		while (std::getline(stream, path))
			filepaths.push_back(path);

		for (const auto& file : filepaths)
			ModelManager::LoadModel(file);
	}

	void EditorLayer::LoadTextures()
	{
		std::ifstream stream("src/Textures.txt");
		std::string path;
		std::vector<std::string> filepaths;

		while (std::getline(stream, path))
			filepaths.push_back(path);

		for (const auto& file : filepaths)
			TextureManager::LoadTexture(Texture2D::Create(file));
	}

	void EditorLayer::LoadShaders()
	{
		std::ifstream stream("src/Shaders.txt");
		std::string path;
		std::vector<std::string> filepaths;

		while (std::getline(stream, path))
			filepaths.push_back(path);

		for (const auto& file : filepaths)
			ShaderLibrary::Load(file);
	}

	void EditorLayer::SaveSceneAs()
	{
		const std::string& path = FileDialog::SaveFile("Engine3D Scene (*.e3d)\0*.e3d\0");

		if (!path.empty())
		{
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(path);
		}
	}

	void EditorLayer::NewScene()
	{
		m_Scene = CreateRef<Scene>(m_Skybox);
		m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		m_Scene->GetCamera().SetProjection(65.0f, m_ViewportSize.x / m_ViewportSize.y);
		m_SceneGraphPanel.SetScene(m_Scene);
	}

	void EditorLayer::OpenScene()
	{
		const std::string& path = FileDialog::OpenFile("Engine3D Scene (*.e3d)\0*.e3d\0");

		if (!path.empty())
		{
			m_Scene = CreateRef<Scene>(m_Skybox);
			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->GetCamera().SetProjection(65.0f, m_ViewportSize.x / m_ViewportSize.y);
			m_SceneGraphPanel.SetScene(m_Scene);

			SceneSerializer serializer(m_Scene);
			serializer.Deserialize(path);
		}
	}
}