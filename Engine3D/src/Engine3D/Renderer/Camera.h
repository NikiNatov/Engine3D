#pragma once

#include <glm\glm.hpp>

namespace E3D
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);

		virtual ~Camera() = default;

		void SetProjection(float fov, float aspectRatio);

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		glm::mat4& GetProjection() { return m_ProjectionMatrix; }
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}