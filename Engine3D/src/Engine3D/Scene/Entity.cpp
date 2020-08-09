#include "pch.h"
#include "Entity.h"

namespace E3D
{
	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{
	}
}