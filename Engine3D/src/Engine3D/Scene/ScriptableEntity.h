#pragma once

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Core\Timestep.h"

namespace E3D
{
	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity->GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnDestroy() {}
	private:
		Ref<Entity> m_Entity;
	};

}