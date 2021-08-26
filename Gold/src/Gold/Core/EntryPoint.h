#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern Gold::Application* Gold::CreateApplication();

int main(int argc, char** argv)
{
	//Rave::Log::Init();

	//RV_PROFILE_BEGIN_SESSION("Startup", "RaveProfile-Startup.json");
	auto app = Gold::CreateApplication();
	//RV_PROFILE_END_SESSION();

	//RV_PROFILE_BEGIN_SESSION("Runtime", "RaveProfile-Runtime.json");
	app->Run();
	//RV_PROFILE_END_SESSION();

	//RV_PROFILE_BEGIN_SESSION("Shutdown", "RaveProfile-Shutdown.json");
	delete app;
	//RV_PROFILE_END_SESSION();
}
#endif