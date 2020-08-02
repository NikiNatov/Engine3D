#pragma once

#include "Engine3D\Log.h"

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