#pragma once

#include "spdlog\spdlog.h"

#include <memory>

namespace E3D {

	class Log
	{
	public:
		Log() = default;
		~Log() = default;

		static void Init();

		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

#define E3D_CORE_LOG_TRACE(...)			E3D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define E3D_CORE_LOG_INFO(...)			E3D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define E3D_CORE_LOG_WARNING(...)		E3D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define E3D_CORE_LOG_ERROR(...)			E3D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define E3D_CORE_LOG_CRITICAL(...)		E3D::Log::GetCoreLogger()->critical(__VA_ARGS__)
										
#define E3D_LOG_TRACE(...)				E3D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define E3D_LOG_INFO(...)				E3D::Log::GetClientLogger()->info(__VA_ARGS__)
#define E3D_LOG_WARNING(...)			E3D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define E3D_LOG_ERROR(...)				E3D::Log::GetClientLogger()->error(__VA_ARGS__)
#define E3D_LOG_CRITICAL(...)			E3D::Log::GetClientLogger()->critical(__VA_ARGS__)
