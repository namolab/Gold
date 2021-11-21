#pragma once
#include "Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Gold {

	class Log
	{
	public:
		static void Init();
		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}

//core log macros
#define GD_CORE_TRACE(...)	::Gold::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GD_CORE_INFO(...)	::Gold::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GD_CORE_WARN(...)	::Gold::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GD_CORE_ERROR(...)	::Gold::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GD_CORE_CRITICAL(...)	::Gold::Log::GetCoreLogger()->critical(__VA_ARGS__)

//client log macros
#define GD_TRACE(...)	::Gold::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GD_INFO(...)	::Gold::Log::GetClientLogger()->info(__VA_ARGS__)
#define GD_WARN(...)	::Gold::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GD_ERROR(...)	::Gold::Log::GetClientLogger()->error(__VA_ARGS__)
#define GD_CRITICAL(...)	::Gold::Log::GetClientLogger()->critical(_VA__ARGS__)

//