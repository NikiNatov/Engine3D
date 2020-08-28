#pragma once

#include "Engine3D.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace E3D
{
	class Player : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{

		}

		virtual void OnUpdate(Timestep ts) override
		{
			auto& transform = GetComponent<TransformComponent>().Transform;
			float speed = 50.0f;
			float rotation = 180.0f;

			if (Input::IsKeyPressed(E3D_KEY_UP))
			{
				transform[3][2] += speed * ts;
			}
			else if (Input::IsKeyPressed(E3D_KEY_DOWN))
			{
				transform[3][2] -= speed * ts;
			}
			if (Input::IsKeyPressed(E3D_KEY_LEFT))
			{
				transform = glm::rotate(transform, glm::radians(rotation * ts), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (Input::IsKeyPressed(E3D_KEY_RIGHT))
			{
				transform = glm::rotate(transform, glm::radians(-rotation * ts), glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}

		virtual void OnDestroy() override
		{

		}
	private:
		double m_A;
	};
}