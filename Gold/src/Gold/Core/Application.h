#pragma once

#include "Core.h"

namespace Gold {

	class GOLD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}
