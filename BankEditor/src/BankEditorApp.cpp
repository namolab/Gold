#include <Gold.h>
#include <Gold/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Gold {

	class BankEditor : public Application
	{
	public:
		BankEditor(ApplicationCommandLineArgs args)
			:Application("BankEditor", args)
		{
			PushLayer(new EditorLayer());
		}

		~BankEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new BankEditor(args);
	}

}