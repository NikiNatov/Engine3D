#pragma once

#include "Engine3D\Core\Config.h"

#include <glm\glm.hpp>
#include <entt.hpp>

namespace E3D
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			E3D_CORE_ASSERT(!HasComponent<T>(), "Component already exists!");
			return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			E3D_CORE_ASSERT(HasComponent<T>(), "Component does not exist!");
			m_Scene->m_Registry.remove<T>(m_Entity);
		}

		template<typename T>
		T& GetComponent()
		{
			E3D_CORE_ASSERT(HasComponent<T>(), "Component does not exist!");
			return m_Scene->m_Registry.get<T>(m_Entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Entity);
		}

		operator bool() const { return m_Entity != entt::null; }

		void AddChild(Ref<Entity>& child);
		void RemoveChild(Ref<Entity>& child);

		void DisplayTree(Entity*& selectedEntity);
		void DisplayEntityProperties();

		void Render(const glm::mat4& parentTransform);
	private:
		entt::entity m_Entity{ entt::null };
		Scene* m_Scene = nullptr;
		std::vector<Ref<Entity>> m_Children;
	};
}