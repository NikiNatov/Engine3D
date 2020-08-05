#include "Engine3D.h"

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{	
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

		E3D::BufferLayout layout = {
			{ "a_Position", E3D::DataType::Float3, false },
			{ "a_Color", E3D::DataType::Float3, false }
		};

		m_QuadVertexArray = E3D::VertexArray::Create();
		m_QuadVertexArray->Bind();

		m_QuadVertexBuffer = E3D::VertexBuffer::Create(quad, sizeof(quad));
		m_QuadVertexBuffer->SetLayout(layout);

		m_QuadIndexBuffer = E3D::IndexBuffer::Create(quadIndices, 6);

		m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);
		m_QuadVertexArray->SetIndexBuffer(m_QuadIndexBuffer);
	}

	virtual void OnUpdate() override
	{
		E3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		E3D::RenderCommand::ClearScreen();

		m_PositionColorShader->Bind();
		E3D::Renderer::Submit(m_QuadVertexArray);
	}

	virtual void OnEvent(E3D::Event& event) override
	{
		
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
	E3D::Ref<E3D::VertexBuffer> m_QuadVertexBuffer;
	E3D::Ref<E3D::IndexBuffer> m_QuadIndexBuffer;
	E3D::Ref<E3D::VertexArray> m_QuadVertexArray;
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