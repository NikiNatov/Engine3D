#pragma once

#include "Engine3D\Scene\SceneCamera.h"
#include "Engine3D\Renderer\Mesh.h"

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
		Ref<Model> Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent& other) = default;
		MeshComponent(const std::string& filepath)
		{
			Mesh = CreateRef<Model>(filepath);
		}
		MeshComponent(const Ref<Model>& mesh)
			: Mesh(mesh)
		{
		}
	};

}