#include "pch.h"
#include "Entity.h"

namespace E3D
{
	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{
	}

	bool Entity::HasComponent(uint32_t id)
	{
		uint32_t types[] = { id };
		return m_Scene->m_Registry.runtime_view(std::cbegin(types), std::cend(types)).contains(m_Entity);
	}
}