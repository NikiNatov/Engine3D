#pragma once

#include <glm\glm.hpp>

#include <string>

namespace E3D
{
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

	struct MeshComponent
	{

	};

	// Temporary
	struct ColorComponent
	{
		glm::vec3 Color{ 1.0f };

		ColorComponent() = default;
		ColorComponent(const ColorComponent& other) = default;
		ColorComponent(const glm::vec3& color)
			: Color(color)
		{
		}
	};
}