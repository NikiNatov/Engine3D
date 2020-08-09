#pragma once

#include "Engine3D\Core\Log.h"

#include <memory>

#ifdef E3D_DEBUG
	#define E3D_ENABLE_ASSERTS
#endif

#ifdef E3D_ENABLE_ASSERTS
	#define E3D_CORE_ASSERT(x,...) { if(!x) { E3D_CORE_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define E3D_ASSERT(x,...) { if(!x) { E3D_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define E3D_CORE_ASSERT(x,...)
	#define E3D_ASSERT(x,...)
#endif

#define BIT(x) (1 << x)

#define E3D_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace E3D
{
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}