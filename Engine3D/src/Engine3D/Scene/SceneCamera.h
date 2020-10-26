#pragma once

#include "Engine3D\Renderer\Camera.h"

namespace E3D
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};
	public:
		SceneCamera();
		~SceneCamera() = default;

		void SetPerspective(float fov, float nearPlane, float farPlane);
		void SetOrthographic(float size, float nearPlane, float farPlane);

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		inline void SetPerspectiveNear(float value) { m_PerspectiveNear = value; RecalculateProjection(); }
		inline void SetPerspectiveFar(float value) { m_PerspectiveFar = value; RecalculateProjection(); }
		inline void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }

		inline void SetOrthographicNear(float value) { m_OrthographicNear = value; RecalculateProjection(); }
		inline void SetOrthographicFar(float value) { m_OrthographicFar = value; RecalculateProjection(); }
		inline void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthographicNear; }
		float GetOrthographicFar() const { return m_OrthographicFar; }
		float GetOrthographicSize() const { return m_OrthographicSize; }
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = 45.0f;
		float m_AspectRatio = 16.0f / 9.0f;
		float m_PerspectiveNear = 0.1f;
		float m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = 1000.0f, m_OrthographicFar = -1000.0f;

	};
}