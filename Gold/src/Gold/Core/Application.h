#pragma once

#include "Gold/Core/Base.h"
#include "Window.h"
#include "Gold/Core/LayerStack.h"
#include "Gold/Events/Event.h"
#include "Gold/Events/ApplicationEvent.h"
#include "Gold/Core/Timestep.h"

#include "Gold/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Gold {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			return Args[index];
		}
	};


	class Application
	{
	public:
		Application(const std::string& name = "Gold App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(std::string m_DebugName);
		void PopOverlay(std::string m_DebugName);

		Window& GetWindow() { return *m_Window; }

		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }


		inline static Application& Get() { return *s_Instance; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		LayerStack GetLayerStack() { return m_LayerStack; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//클라이언트 안에서 정의
	Application* CreateApplication(ApplicationCommandLineArgs args);

}