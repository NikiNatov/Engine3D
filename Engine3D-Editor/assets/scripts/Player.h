#pragma once

#include "Engine3D.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

namespace E3D
{
	class Player : public ScriptableEntity
	{
	public:
		glm::quat GetOrientation()
		{
			return glm::normalize(glm::angleAxis(GetComponent<TransformComponent>().Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::angleAxis(GetComponent<TransformComponent>().Rotation.z, glm::vec3(1.0f, 0.0f, 0.0f)));
		}
		glm::vec3 GetUp()
		{
			return glm::normalize(GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f));
		}

		glm::vec3 GetRight()
		{
			return glm::normalize(GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glm::vec3 GetFront()
		{
			return glm::normalize(GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
		}

		virtual void OnCreate() override
		{

		}

		virtual void OnUpdate(Timestep ts) override
		{
			auto& tc = GetComponent<TransformComponent>();
			float speed = 50.0f;
			float rotation = 180.0f;

			glm::vec3 forward = { 0.0f, 0.0f, -1.0f };
			glm::vec3 up = { 0.0f, 1.0f, 0.0f };
			glm::vec3 right = { 1.0f, 0.0f, 0.0f };

			if (Input::IsKeyPressed(E3D_KEY_UP))
			{
				tc.Translation += GetFront() * speed * (float)ts;
				tc.Translation += GetUp() * speed * (float)ts * sin(GetComponent<TransformComponent>().Rotation.x);
			}
			else if (Input::IsKeyPressed(E3D_KEY_DOWN))
			{
				tc.Translation -= GetFront() * speed * (float)ts;
				tc.Translation -= GetUp() * speed * (float)ts * sin(GetComponent<TransformComponent>().Rotation.x);
			}
			if (Input::IsKeyPressed(E3D_KEY_LEFT))
			{
				tc.Rotation.y += glm::radians(rotation * ts);
			}
			else if (Input::IsKeyPressed(E3D_KEY_RIGHT))
			{
				tc.Rotation.y -= glm::radians(rotation * ts);
			}
			if (Input::IsKeyPressed(E3D_KEY_I))
			{
				tc.Rotation.x += glm::radians(rotation * ts);
			}
			else if (Input::IsKeyPressed(E3D_KEY_K))
			{
				tc.Rotation.x -= glm::radians(rotation * ts);
			}
		}

		virtual void OnDestroy() override
		{

		}
	};
}