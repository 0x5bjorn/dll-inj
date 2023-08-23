#include <iostream>

#include "Proc/ProcHandler.h"
#include "Application.h"

int main()
{
	//Proc::GetProcessList();
	Proc::GetProcessModulesList(17528);

	Application* app = new Application("dll-inj", 1280, 720);

	app->Run();

	delete app;
}