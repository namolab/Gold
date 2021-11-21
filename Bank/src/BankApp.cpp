#include <Gold.h>

#include "Platform//OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Bank2D.h"
#include <Gold/Core/EntryPoint.h>

namespace Gold {

	class Bank : public Application
	{
	public:
		Bank(ApplicationCommandLineArgs args)
			:Application("BankApp",args)
		{
			PushLayer(new Bank2D());
		}

		~Bank()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Bank(args);
	}

}