#include "Engine3D.h"

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{	
		E3D::Renderer::Init();
		m_CameraController = std::make_unique<E3D::PerspectiveCameraController>(45.0f, (float)1280 / (float)720);

		m_PositionColorShader = E3D::Shader::Create("assets/shaders/PositionColorShader.glsl");
		m_PositionColorShader->SetMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjection());


		float cubeVertices[] =
		{
					-0.5f, 0.5f,-3.5f,  1.0f, 0.0f, 0.0f,
					-0.5f,-0.5f,-3.5f,	0.0f, 1.0f, 0.0f,
					 0.5f,-0.5f,-3.5f,	0.0f, 0.0f, 1.0f,
					 0.5f, 0.5f,-3.5f,	0.5f, 0.5f, 0.0f,

					-0.5f, 0.5f, -2.5f, 1.0f, 0.0f, 0.0f,
					-0.5f,-0.5f, -2.5f, 0.0f, 1.0f, 0.0f,
					 0.5f,-0.5f, -2.5f, 0.0f, 0.0f, 1.0f,
					 0.5f, 0.5f, -2.5f, 0.5f, 0.5f, 0.0f,		
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
			{ "a_Color", E3D::DataType::Float3, false }
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
		m_CameraController->OnUpdate(ts);
		m_PositionColorShader->SetMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjection());

		E3D::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		E3D::RenderCommand::ClearScreen();

		m_PositionColorShader->Bind();
		E3D::Renderer::Submit(m_CubeVertexArray);
	}

	virtual void OnEvent(E3D::Event& event) override
	{
	
		m_CameraController->OnEvent(event);
	}

	virtual void OnImGuiRender() override
	{

	}

	virtual void OnAttach() override
	{

	}

	virtual void OnDetach() override
	{

	}
private:
	E3D::Ref<E3D::Shader> m_PositionColorShader;
	E3D::Ref<E3D::VertexBuffer> m_CubeVertexBuffer;
	E3D::Ref<E3D::IndexBuffer> m_CubeIndexBuffer;
	E3D::Ref<E3D::VertexArray> m_CubeVertexArray;

	E3D::Scope<E3D::PerspectiveCameraController> m_CameraController;
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