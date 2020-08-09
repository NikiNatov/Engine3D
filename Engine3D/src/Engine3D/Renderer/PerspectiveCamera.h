#pragma once

#include <glm\glm.hpp>

namespace E3D
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, const glm::vec3& position = glm::vec3(0.0f), float pitch = 0.0f, float yaw = -89.0f);
		~PerspectiveCamera() = default;

		void SetProjection(float fov, float aspectRatio);
		void SetPosition(const glm::vec3& position);
		void SetAngles(float pitch, float yaw);

		inline const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjection() const { return m_ViewProjection; }

		inline const glm::vec3& GetPositiveXAxis() const { return m_CameraXAxis; }
		inline const glm::vec3& GetPositiveYAxis() const { return m_CameraYAxis; }
		inline const glm::vec3& GetPositiveZAxis() const { return m_CameraZAxis; }
	private:
		void UpdateVectors();
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjection;

		glm::vec3 m_Position;
		glm::vec3 m_Direction;

		glm::vec3 m_CameraXAxis;
		glm::vec3 m_CameraYAxis;
		glm::vec3 m_CameraZAxis;

		float m_Fov;
		float m_AspectRatio;

		float m_Pitch;
		float m_Yaw;
	};
}