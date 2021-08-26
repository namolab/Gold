#pragma once
#include <memory>

#ifdef GD_PLATFORM_WINDOWS
	#ifdef GD_BUILD_DLL
		#define GOLD_API __declspec(dllexport)
	#else
		#define GOLD_API __declspec(dllimport)
	#endif
#else
	#error Gold only supports Window 
#endif 