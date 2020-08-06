#include "pch.h"
#include "PerspectiveCameraController.h"

#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\KeyCodes.h"
#include "Engine3D\Core\MouseCodes.h"

#include <glad\glad.h>

namespace E3D
{
	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_Camera(m_Fov, m_AspectRatio)
	{
	}
	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(E3D_KEY_W))
			m_CameraPosition -= m_Camera.GetPositiveZAxis() * m_MovementSpeed * (float)ts;
		else if (Input::IsKeyPressed(E3D_KEY_S))
			m_CameraPosition += m_Camera.GetPositiveZAxis() * m_MovementSpeed * (float)ts;
		if (Input::IsKeyPressed(E3D_KEY_A))
			m_CameraPosition -= m_Camera.GetPositiveXAxis() * m_MovementSpeed * (float)ts;
		else if (Input::IsKeyPressed(E3D_KEY_D))
			m_CameraPosition += m_Camera.GetPositiveXAxis() * m_MovementSpeed * (float)ts;

		m_Camera.SetPosition(m_CameraPosition);
		
	}
	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseMovedEvent>(E3D_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(E3D_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizedEvent>(E3D_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}
	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		static bool first = true;
	
		float currentX = event.GetXPosition();
		float currentY = event.GetYPosition();

		if (first)
		{
			m_LastScreenXPosition = currentX;
			m_LastScreenYPosition = currentY;
			first = false;
		}

		float xOffset = currentX - m_LastScreenXPosition;
		float yOffset = currentY - m_LastScreenYPosition;

		m_LastScreenXPosition = currentX;
		m_LastScreenYPosition = currentY;

		if (Input::IsMouseButtonPressed(E3D_MOUSE_BUTTON_LEFT))
		{
			m_Pitch -= yOffset * m_MouseSensitivity;
			m_Yaw += xOffset * m_MouseSensitivity;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			else if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			m_Camera.SetAngles(m_Pitch, m_Yaw);
		}

		return false;
	}
	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_Zoom -= event.GetYOffset();
		m_Camera.SetProjection(m_Fov + m_Zoom, m_AspectRatio);

		return false;
	}
	bool PerspectiveCameraController::OnWindowResized(WindowResizedEvent& event)
	{
		// Temporary
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(m_Fov, m_AspectRatio);

		return false;
	}
}