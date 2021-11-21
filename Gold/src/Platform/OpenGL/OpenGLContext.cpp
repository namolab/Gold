#include "hzpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>

#include <glad/glad.h>

namespace Gold {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		GD_CORE_ASSERT(windowHandle, "Window Handle is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GD_CORE_ASSERT(status, "Failed to initalize Glad!");
		GD_CORE_INFO("OpenGL Info ->");
		GD_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		GD_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		GD_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

