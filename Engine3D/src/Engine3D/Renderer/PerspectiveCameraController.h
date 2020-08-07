#pragma once

#include "Engine3D\Renderer\PerspectiveCamera.h"
#include "Engine3D\Core\Timestep.h"

#include "Engine3D\Events\Event.h"
#include "Engine3D\Events\ApplicationEvent.h"
#include "Engine3D\Events\KeyEvent.h"
#include "Engine3D\Events\MouseEvent.h"

namespace E3D
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio);
		~PerspectiveCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		inline const PerspectiveCamera& GetCamera() const { return m_Camera; }
		inline PerspectiveCamera& GetCamera() { return m_Camera; }

		inline const glm::vec3& GetPosition() const { return m_CameraPosition; }
		inline glm::vec3& GetPosition() { return m_CameraPosition; }
	private:
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizedEvent& event);
	private:
		float m_Fov;
		float m_AspectRatio;
		float m_Pitch = 0.0f;
		float m_Yaw = -89.0f;
		PerspectiveCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.2f;
		float m_Zoom = 0.0f;

		float m_LastScreenXPosition = 0.0f;
		float m_LastScreenYPosition = 0.0f;
	};
}