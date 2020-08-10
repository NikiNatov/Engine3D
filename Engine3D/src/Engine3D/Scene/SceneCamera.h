#pragma once

#include "Engine3D\Renderer\Camera.h"

namespace E3D
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		~SceneCamera() = default;

		void SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetFov() const { return m_Fov; }
		inline void SetFov(float fov) { m_Fov = fov; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_Fov = 45.0f;
		float m_AspectRatio = 0.0f;
		float m_NearPlane = 0.01f;
		float m_FarPlane = 10000.0f;
	};
}