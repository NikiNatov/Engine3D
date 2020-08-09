#include "pch.h"
#include "PerspectiveCamera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace E3D
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, const glm::vec3& position, float pitch, float yaw)
		: m_Position(position), m_Direction(0.0f, 0.0f, -1.0f), m_Fov(fov), m_AspectRatio(aspectRatio), m_Pitch(pitch), m_Yaw(yaw)
	{
		SetProjection(fov, aspectRatio);
		UpdateVectors();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.0f);
		m_ViewProjection = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateVectors();
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetAngles(float pitch, float yaw)
	{
		m_Pitch = pitch;
		m_Yaw = yaw;
	}

	void PerspectiveCamera::UpdateVectors()
	{
		glm::vec3 newDir;
		newDir.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		newDir.y = sin(glm::radians(m_Pitch));
		newDir.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

		m_Direction = glm::normalize(newDir);

		m_CameraZAxis = glm::normalize(-m_Direction);
		m_CameraXAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_CameraZAxis));
		m_CameraYAxis = glm::normalize(glm::cross(m_CameraZAxis, m_CameraXAxis));	
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
		m_ViewProjection = m_ProjectionMatrix * m_ViewMatrix;
	}
}