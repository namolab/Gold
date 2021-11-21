#pragma once
#include <memory>

#ifdef GD_PLATFORM_WINDOWS
#if GD_DYNAMIC_LINK
	#ifdef GD_BUILD_DLL
		#define GOLD_API __declspec(dllexport)
	#else
		#define GOLD_API __declspec(dllimport)
	#endif
#else
	#define GOLD_API
#endif
#else
	#error Gold only supports Window 
#endif 

#ifdef GD_DEBUG
	#define GD_ENABLE_ASSERTS
#endif

#ifdef GD_ENABLE_ASSERTS 
	#define GD_ASSERT(x, ...) { if(!(x)) { GD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
	#define GD_CORE_ASSERT(x, ...) { if(!(x)) { GD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
#else 
	#define GD_ASSERT(x, ...) 
	#define GD_CORE_ASSERT(x, ...) 
#endif 

#define BIT(x) (1<<x)

#define GD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Gold {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}