#include "pch.h"
#include "Scene.h"

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Components.h"

#include "Engine3D\Renderer\Renderer.h"

namespace E3D
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(name);

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
	}
}