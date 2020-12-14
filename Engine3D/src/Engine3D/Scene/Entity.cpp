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
		{
			sceneNode.FirstChild = child;
		}
		else
		{
			Entity currentChild = sceneNode.FirstChild;
			while (currentChild.GetComponent<SceneNodeComponent>().NextSibling)
				currentChild = currentChild.GetComponent<SceneNodeComponent>().NextSibling;

			currentChild.GetComponent<SceneNodeComponent>().NextSibling = child;
			child.GetComponent<SceneNodeComponent>().PreviousSibling = currentChild;
		}

		child.GetComponent<SceneNodeComponent>().Parent = *this;
	}

	void Entity::RemoveChildren()
	{
		Entity current = GetComponent<SceneNodeComponent>().FirstChild;

		if (!current)
		{
			return;
		}

		while (current)
		{
			Entity temp = current;
			current = current.GetComponent<SceneNodeComponent>().NextSibling;

			Entity parent = GetComponent<SceneNodeComponent>().Parent;
			if (parent)
			{
				if (temp == parent.GetComponent<SceneNodeComponent>().FirstChild)
					parent.GetComponent<SceneNodeComponent>().FirstChild == temp.GetComponent<SceneNodeComponent>().NextSibling;
				else
				{
					Entity prev = temp.GetComponent<SceneNodeComponent>().PreviousSibling;
					Entity next = temp.GetComponent<SceneNodeComponent>().NextSibling;

					if(prev)
						prev.GetComponent<SceneNodeComponent>().NextSibling = next;

					if(next)
						next.GetComponent<SceneNodeComponent>().PreviousSibling = prev;
				}
			}

			temp.RemoveChildren();
			m_Scene->m_Registry.destroy((entt::entity)(temp));
		}
	}

	bool Entity::HasComponent(uint32_t id)
	{
		uint32_t types[] = { id };
		return m_Scene->m_Registry.runtime_view(std::cbegin(types), std::cend(types)).contains(m_Entity);
	}
}