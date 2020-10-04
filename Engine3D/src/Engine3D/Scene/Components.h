#pragma once

#include "Engine3D\Scene\SceneCamera.h"
#include "Engine3D\Renderer\Mesh.h"
#include "Engine3D\Scene\ScriptableEntity.h"

#include <glm\glm.hpp>

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
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{
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