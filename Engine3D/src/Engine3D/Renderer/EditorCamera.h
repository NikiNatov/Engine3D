#pragma once

#include "Engine3D/Renderer/Camera.h"
#include "Engine3D\Core\Timestep.h"
#include "Engine3D\Events\Event.h"
#include "Engine3D\Events\MouseEvent.h"

#include <glm\glm.hpp>

namespace E3D
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const glm::mat4& projectionMatrix);

		void Update(Timestep ts);
		void OnEvent(Event& event);

		inline void Translate(const glm::vec3& translation) { m_Position += translation; }
		inline void Rotate(const glm::vec3& rotation) { m_Rotation += rotation; }

		inline void SetTargetPoint(const glm::vec3& target) { m_TargetPoint = target; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		inline float GetDistanceFromTarget() const { return m_Distance; }
		inline const glm::vec3& GetTargetPoint() const { return m_TargetPoint; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);

		void MouseMove(const glm::vec2& mouseDelta, Timestep ts);
		void MouseRotate(const glm::vec2& mouseDelta, Timestep ts);
		void MouseZoom(float zoomDelta, Timestep ts);

		glm::vec3 CalculatePosition() const;

		glm::quat GetOrientation() const;

		glm::vec3 GetCameraUp() const;
		glm::vec3 GetCameraRight() const;
		glm::vec3 GetCameraFront() const;
	private:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_TargetPoint;
		glm::vec3 m_Position, m_Rotation;

		glm::vec2 m_LastMousePosition = glm::vec2(0.0f);

		float m_Distance;
		float m_Pitch, m_Yaw;

		float m_MoveSpeed, m_RotationSpeed, m_ZoomSpeed;
	};
}