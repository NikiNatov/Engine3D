#pragma once

#include "Engine3D\Core\Timestep.h"

#include <entt.hpp>


namespace E3D
{
	class Entity;

	class Scene
	{
		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}