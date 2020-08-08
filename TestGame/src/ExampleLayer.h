#pragma once
#include <Engine3D.h>

#include <ImGui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(E3D::Timestep ts) override;

	virtual void OnEvent(E3D::Event& event) override;

	virtual void OnImGuiRender() override;

	virtual void OnAttach() override;

	virtual void OnDetach() override;
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
};
