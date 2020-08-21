#include "pch.h"
#include "Scene.h"

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Components.h"

#include "Engine3D\Renderer\Renderer.h"

#include "glm\gtc\type_ptr.hpp"

#include <imgui.h>
#include <ImGuizmo.h>


namespace E3D
{
	Scene::Scene()
		: m_SceneGraph(this)
	{
		m_TestEntity = CreateEntity("Darth Vader");
		m_TestEntity->AddComponent<MeshComponent>("assets/models/starwars/darth vader/vaderModified.fbx");
		auto& transform = m_TestEntity->GetComponent<TransformComponent>().Transform;


		m_MainCamera = CreateEntity("Main Camera");
		m_MainCamera->AddComponent<MeshComponent>("assets/models/camera/camera.obj");
		m_MainCamera->AddComponent<CameraComponent>().Primary = true;
		auto& cameraTransform = m_MainCamera->GetComponent<TransformComponent>().Transform;
		cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 35.0f));

		m_SceneGraph.GetRoot()->AddChild(m_MainCamera);

		m_AnotherEntity = CreateEntity("Tie-Fighter");
		m_AnotherEntity->AddComponent<MeshComponent>("assets/models/starwars/spaceship/tie-fighter.fbx");
		auto& transform2 = m_AnotherEntity->GetComponent<TransformComponent>().Transform;
		transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f));

		m_Anakin = CreateEntity("Anakin Skywalker");
		m_Anakin->AddComponent<MeshComponent>("assets/models/starwars/anakin/0.dae");


		m_SceneGraph.GetRoot()->AddChild(m_AnotherEntity);
		m_SceneGraph.GetRoot()->AddChild(m_Anakin);
		m_AnotherEntity->AddChild(m_TestEntity);
	}

	Scene::~Scene()
	{
	}

	Ref<Entity> Scene::CreateEntity(const std::string& name)
	{
		Ref<Entity> entity = CreateRef<Entity>(m_Registry.create(), this);
		entity->AddComponent<TransformComponent>();
		entity->AddComponent<NameComponent>(name);

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		
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

	void Scene::OnEditRender()
	{
		m_SceneGraph.GetRoot()->Render(glm::mat4(1.0f));
		/*auto view = m_Registry.view<TransformComponent, MeshComponent>();

		for (auto entity : view)
		{
			auto [transform, mesh] = view.get<TransformComponent, MeshComponent>(entity);
			mesh.Mesh->Draw(transform.Transform);
		}*/
	}

	void Scene::OnRunningRender()
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent>();

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		for (auto entity : view)
		{
			auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

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

			m_SceneGraph.GetRoot()->Render(glm::mat4(1.0f));

			/*auto view = m_Registry.view<TransformComponent, MeshComponent>();

			for (auto entity : view)
			{
				auto& [transform, mesh] = view.get<TransformComponent, MeshComponent>(entity);
				mesh.Mesh->Draw(transform.Transform);
			}*/
		}
	}
	
}