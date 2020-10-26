#include "pch.h"
#include "Entity.h"

#include "Engine3D\Scene\Components.h"

namespace E3D
{
	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{
	}

	Entity::Entity(uint32_t entity, Scene* scene)
		: m_Entity((entt::entity)entity), m_Scene(scene)
	{
	}

	void Entity::AddChild(Entity child)
	{
		auto& sceneNode = GetComponent<SceneNodeComponent>();
		if (!sceneNode.FirstChild)
			sceneNode.FirstChild = child;
		else
		{
			Entity currentChild = sceneNode.FirstChild;
			while (currentChild.GetComponent<SceneNodeComponent>().NextSibling)
				currentChild = currentChild.GetComponent<SceneNodeComponent>().NextSibling;

			currentChild.GetComponent<SceneNodeComponent>().NextSibling = child;
		}

		child.GetComponent<SceneNodeComponent>().Parent = *this;
	}

	bool Entity::HasComponent(uint32_t id)
	{
		uint32_t types[] = { id };
		return m_Scene->m_Registry.runtime_view(std::cbegin(types), std::cend(types)).contains(m_Entity);
	}
}