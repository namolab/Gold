#pragma once
#include "Gold/Core/Base.h"
#include "Gold/Core/Application.h"

#ifdef GD_PLATFORM_WINDOWS

extern Gold::Application* Gold::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Gold::Log::Init();

	auto app = Gold::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}
#endif