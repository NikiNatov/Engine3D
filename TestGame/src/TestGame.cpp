#include "Engine3D.h"
#include <ImGui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{	
		E3D::Renderer::Init();
		m_CameraController = std::make_unique<E3D::PerspectiveCameraController>(45.0f, (float)1280 / (float)720);

		auto posColorShader = m_ShaderLibrary.Load("assets/shaders/PositionColorShader.glsl");
		posColorShader->SetMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjection());

		m_ExampleTexture = E3D::Texture2D::Create("assets/textures/container.jpg");

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
		textureShader->SetMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjection());
		textureShader->SetInt("u_Texture", 0);

		float cubeVertices[] =
		{
			-0.5f,  0.5f, -3.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -3.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -3.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -3.5f, 1.0f, 1.0f,

			-0.5f,  0.5f, -2.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -2.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -2.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -2.5f, 1.0f, 1.0f		
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

		m_CameraController->OnUpdate(ts);

		E3D::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		E3D::RenderCommand::ClearScreen();

		m_ExampleTexture->Bind(0);
		auto textureShader = m_ShaderLibrary.Get("TextureShader");
		textureShader->SetMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjection());
		E3D::Renderer::Submit(m_CubeVertexArray, textureShader, m_CubeTransform);
	}

	virtual void OnEvent(E3D::Event& event) override
	{
	
		m_CameraController->OnEvent(event);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Transform");
		ImGui::SliderFloat3("Translation", &m_CubePosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotation", &m_CubeRotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat3("Scale", &m_CubeScale.x, 0.5f, 3.0f);
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
	glm::mat4 m_CubeTransform = glm::mat4(1.0f);

	E3D::Scope<E3D::PerspectiveCameraController> m_CameraController;
	E3D::Ref<E3D::Texture2D> m_ExampleTexture;

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