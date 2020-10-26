#include "pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace E3D
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearPlane, float farPlane)
	{
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearPlane;
		m_PerspectiveFar = farPlane;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearPlane, float farPlane)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearPlane;
		m_OrthographicFar = farPlane;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (height == 0)
			return;
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if(m_ProjectionType == ProjectionType::Perspective)
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		else
		{
			float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float top = m_OrthographicSize * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}
	}
}