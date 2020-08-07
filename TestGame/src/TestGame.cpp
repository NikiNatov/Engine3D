#include "Engine3D.h"
#include <ImGui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{	
		auto posColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");
		posColorShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());

		m_ExampleTexture = E3D::Texture2D::Create("assets/textures/container.jpg");

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
		textureShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());
		textureShader->SetInt("u_Texture", 0);

		E3D::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = E3D::Framebuffer::Create(fbSpec);

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

		E3D::BufferLayout layout = {
			{ "a_Position", E3D::DataType::Float3, false },
			{ "a_TexCoord", E3D::DataType::Float2, false }
		};

		m_CubeVertexArray = E3D::VertexArray::Create();
		m_CubeVertexArray->Bind();

		m_CubeVertexBuffer = E3D::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		m_CubeVertexBuffer->SetLayout(layout);

		m_CubeIndexBuffer = E3D::IndexBuffer::Create(cubeIndices, 36);

		m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);
		m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);
	}

	virtual void OnUpdate(E3D::Timestep ts) override
	{
		m_CubeTransform = glm::translate(glm::mat4(1.0f), m_CubePosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), m_CubeScale);

		m_CameraController.OnUpdate(ts);

		m_Framebuffer->Bind();
		E3D::Renderer::BeginScene(m_CameraController.GetCamera());

		E3D::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		E3D::RenderCommand::ClearScreen();

		m_ExampleTexture->Bind(0);
		auto shader = m_ShaderLibrary.Get("FlatColorShader");
		shader->SetFloat3("u_Color", m_CubeColor);
		E3D::Renderer::Submit(m_CubeVertexArray, shader, m_CubeTransform);

		E3D::Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	virtual void OnEvent(E3D::Event& event) override
	{
	
		m_CameraController.OnEvent(event);
	}

	virtual void OnImGuiRender() override
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
					E3D::Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		

		ImGui::Begin("Transform");
		ImGui::SliderFloat3("Translation", &m_CubePosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotation", &m_CubeRotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat3("Scale", &m_CubeScale.x, 0.5f, 3.0f);
		ImGui::ColorEdit3("Color", &m_CubeColor.x);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");
		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&panelSize))
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

	virtual void OnAttach() override
	{

	}

	virtual void OnDetach() override
	{

	}
private:
	E3D::ShaderLibrary m_ShaderLibrary;
	E3D::Ref<E3D::VertexBuffer> m_CubeVertexBuffer;
	E3D::Ref<E3D::IndexBuffer> m_CubeIndexBuffer;
	E3D::Ref<E3D::VertexArray> m_CubeVertexArray;
	glm::vec3 m_CubePosition{ 0.0f };
	glm::vec3 m_CubeRotation{ 0.0f };
	glm::vec3 m_CubeScale{ 1.0f };
	glm::vec3 m_CubeColor{ 1.0f };
	glm::mat4 m_CubeTransform = glm::mat4(1.0f);

	E3D::PerspectiveCameraController m_CameraController{ 45.0f, (float)1280 / (float)720 };
	E3D::Ref<E3D::Texture2D> m_ExampleTexture;
	E3D::Ref<E3D::Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize;
};


class TestGame : public E3D::Application
{
public:
	TestGame()
	{
		PushLayer(new ExampleLayer());
	}

	~TestGame()
	{

	}
};

E3D::Application* E3D::Create()
{
	return new TestGame();
}