#pragma once

#include "Engine3D\Scene\SceneCamera.h"
#include "Engine3D\Renderer\Mesh.h"
#include "Engine3D\Scene\ScriptableEntity.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>

namespace E3D
{
	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent& other) = default;
		NameComponent(const std::string& name)
			: Name(name)
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{
		}

		glm::mat4 GetTransform()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f })*
								 glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f })*
								 glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0f });
			
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent & other) = default;
	};

	struct MeshComponent
	{
		Ref<Mesh> Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent& other) = default;
		MeshComponent(const Ref<E3D::Mesh>& mesh)
			: Mesh(mesh)
		{
		}
	};

	struct ScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*CreateScript)();
		void (*DestroyScript)(ScriptComponent*);

		template<typename T>
		void Bind()
		{
			CreateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](ScriptComponent* script) { delete script->Instance; script->Instance = nullptr; };

		}
	};

	struct SceneNodeComponent
	{
		size_t ChildrenCount = 0;
		Entity Parent;
		Entity FirstChild;
		Entity PreviousSibling;
		Entity NextSibling;

		SceneNodeComponent() = default;
		SceneNodeComponent(const SceneNodeComponent& other) = default;
	};

}