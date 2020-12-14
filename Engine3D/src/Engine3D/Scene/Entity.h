#pragma once

#include "Engine3D\Core\Config.h"
#include "Engine3D\Scene\Scene.h"

#include <entt.hpp>

namespace E3D
{
	class Scene;

	class Entity
	{
		friend class EntityInspector;
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(uint32_t entity, Scene* scene);
		Entity(const Entity& other) = default;

		void AddChild(Entity child);
		void RemoveChildren();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			E3D_CORE_ASSERT(!HasComponent<T>(), "Component already exists!");

			T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
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

		bool HasComponent(uint32_t id);

		operator bool() const { return m_Entity != entt::null; }
		operator uint32_t() const { return (uint32_t)m_Entity; }
		operator entt::entity() const { return m_Entity; }

		bool operator==(const Entity& other) const 
		{ 
			return m_Entity == other.m_Entity && m_Scene == other.m_Scene; 
		}
		bool operator!= (const Entity& other) const { return !(*this == other); }
	private:
		entt::entity m_Entity{ entt::null };
		Scene* m_Scene = nullptr;
	};
}