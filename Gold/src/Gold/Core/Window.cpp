#include "hzpch.h"
#include "Gold/Core/Window.h"

#ifdef GD_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Gold
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef GD_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		//RV_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}