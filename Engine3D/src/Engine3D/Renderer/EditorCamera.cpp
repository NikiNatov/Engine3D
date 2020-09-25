#include "pch.h"
#include "EditorCamera.h"

#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\MouseCodes.h"
#include "Engine3D\Core\KeyCodes.h"

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

namespace E3D
{
	EditorCamera::EditorCamera(const glm::mat4& projectionMatrix)
		: Camera(projectionMatrix)
	{
		m_MoveSpeed = 0.1f;
		m_RotationSpeed = 30.00f;
		m_ZoomSpeed = 30.0f;

		m_Position = glm::vec3(50.0f, 60.0f, 100.0f);
		m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		m_TargetPoint = glm::vec3(0.0f);
		m_Distance = glm::distance(m_TargetPoint, m_Position);

		m_Yaw = 25.0f;
		m_Pitch = 25.0f;
	}

	void EditorCamera::Update(Timestep ts)
	{
		const glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = mousePos - m_LastMousePosition;
		m_LastMousePosition = mousePos;

		if (Input::IsKeyPressed(E3D_KEY_LEFT_ALT))
		{
			if (Input::IsMouseButtonPressed(E3D_MOUSE_BUTTON_MIDDLE))
				MouseMove(delta, ts);
			else if (Input::IsMouseButtonPressed(E3D_MOUSE_BUTTON_LEFT))
				MouseRotate(delta, ts);
			else if (Input::IsMouseButtonPressed(E3D_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y, ts);
		}

		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		glm::vec3 eulerAngles = glm::eulerAngles(orientation);
		m_Rotation = eulerAngles;

		glm::mat4 rotationMatrix = glm::mat4_cast(glm::conjugate(orientation));

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * rotationMatrix * glm::translate(glm::mat4(1.0f), -m_Position);
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(E3D_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& event)
	{
		float yOffset = event.GetYOffset();
		//MouseZoom(yOffset);

		return true;
	}

	void EditorCamera::MouseMove(const glm::vec2& mouseDelta, Timestep ts)
	{
		m_TargetPoint += -GetCameraRight() * mouseDelta.x * m_MoveSpeed * m_Distance * (float)ts;
		m_TargetPoint += GetCameraUp() * mouseDelta.y * m_MoveSpeed * m_Distance * (float)ts;
	}

	void EditorCamera::MouseRotate(const glm::vec2& mouseDelta, Timestep ts)
	{
		float sign = GetCameraUp().y < 0.0f ? -1.0f : 1.0f;
		m_Yaw += sign * mouseDelta.x * m_RotationSpeed * (float)ts;
		m_Pitch += mouseDelta.y * m_RotationSpeed * (float)ts;
	}

	void EditorCamera::MouseZoom(float zoomDelta, Timestep ts)
	{
		m_Distance -= zoomDelta * m_ZoomSpeed * ts;

		if (m_Distance < 1.0f)
		{
			m_TargetPoint += GetCameraFront();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_TargetPoint - GetCameraFront() * m_Distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return  glm::normalize(glm::angleAxis(glm::radians(-m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::angleAxis(glm::radians(-m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f)));
	}

	glm::vec3 EditorCamera::GetCameraUp() const
	{
		return glm::normalize(GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetCameraRight() const
	{
		return glm::normalize(GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetCameraFront() const
	{
		return glm::normalize(GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
	}
}