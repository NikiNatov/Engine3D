#include "pch.h"
#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace E3D
{
	Camera::Camera(const glm::mat4& projection)
		: m_ProjectionMatrix(projection)
	{
	}

	void Camera::SetProjection(float fov, float aspectRatio)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.0f);
	}
}