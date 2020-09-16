#pragma once

#include "Engine3D\Core\Timestep.h"
#include "Engine3D\Core\Config.h"

#include "Engine3D\Renderer\PerspectiveCameraController.h"
#include "Engine3D\Renderer\SkyBox.h"

#include <glm\glm.hpp>
#include <entt.hpp>

namespace E3D
{
	class Scene
	{
		friend class Entity;
		friend class SceneGraph;
	public:
		enum class SceneState { Edit = 0, Running = 1 };
	public:
		Scene();
		Scene(const Ref<Skybox>& skybox);
		~Scene();

		Entity CreateEntity(const std::string& name = "Unnamed Entity");

		void OnSceneStart();
		void OnUpdate(Timestep ts);
		void OnSceneEnd();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnEditRender();
		void OnRunningRender();

		inline void SetSceneState(SceneState state) { m_State = state; }
		inline SceneState GetSceneState() const { return m_State; }
		inline PerspectiveCameraController& GetCameraController() { return m_CameraController; }
	private:
		entt::registry m_Registry;
		SceneState m_State = SceneState::Edit;
		PerspectiveCameraController m_CameraController{ 45.0f, (float)1280 / (float)720 };
		Ref<Skybox> m_Skybox;
	
	};

}