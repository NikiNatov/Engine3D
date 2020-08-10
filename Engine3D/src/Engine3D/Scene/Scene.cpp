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
		auto view = m_Registry.view<CameraComponent, TransformComponent>();

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		for (auto entity : view)
		{
			auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}


		if (mainCamera)
		{
			Renderer::BeginScene(*mainCamera, *cameraTransform);
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}