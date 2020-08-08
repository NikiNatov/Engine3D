#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Layer("Example Layer")
{
	
}

void ExampleLayer::OnUpdate(E3D::Timestep ts)
{
	m_CubeTransform = glm::translate(glm::mat4(1.0f), m_CubePosition) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), m_CubeScale);

	m_CameraController.OnUpdate(ts);

	E3D::Renderer::BeginScene(m_CameraController.GetCamera());

	E3D::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	E3D::RenderCommand::ClearScreen();

	m_ExampleTexture->Bind(0);
	auto shader = m_ShaderLibrary.Get("FlatColorShader");
	shader->SetFloat3("u_Color", m_CubeColor);
	E3D::Renderer::Submit(m_CubeVertexArray, shader, m_CubeTransform);

	E3D::Renderer::EndScene();
}

void ExampleLayer::OnEvent(E3D::Event& event)
{
	m_CameraController.OnEvent(event);
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnAttach()
{
	auto posColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColorShader.glsl");
	posColorShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());

	m_ExampleTexture = E3D::Texture2D::Create("assets/textures/container.jpg");

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
	textureShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjection());
	textureShader->SetInt("u_Texture", 0);

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

void ExampleLayer::OnDetach()
{
}
