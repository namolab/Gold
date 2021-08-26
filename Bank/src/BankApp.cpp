#include <Gold.h>
#include <Gold/Core/EntryPoint.h>

class Bank :public Gold::Application
{
public:
	Bank()
	{
	}

	~Bank()
	{
	}
};


Gold::Application* Gold::CreateApplication()
{
	return new Bank();
}
