#pragma once

#include "Engine3D\Core\Timestep.h"
#include "Engine3D\Core\Config.h"

#include "Engine3D\Scene\SceneGraph.h"

#include <glm\glm.hpp>
#include <entt.hpp>

namespace E3D
{
	class Scene
	{
		friend class Entity;
	public:
		enum class SceneState { Edit = 0, Running = 1 };
	public:
		Scene();
		~Scene();

		Ref<Entity> CreateEntity(const std::string& name = "Unnamed Entity");

		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnEditRender();
		void OnRunningRender();

		inline void SetSceneState(SceneState state) { m_State = state; }
		inline SceneState GetSceneState() const { return m_State; }
		inline SceneGraph& GetSceneGraph() { return m_SceneGraph; }
	private:
		entt::registry m_Registry;
		SceneState m_State = SceneState::Edit;
		SceneGraph m_SceneGraph;

		Ref<Entity> m_TestEntity;
		Ref<Entity> m_MainCamera;
		Ref<Entity> m_AnotherEntity;
		Ref<Entity> m_Anakin;
	};

}