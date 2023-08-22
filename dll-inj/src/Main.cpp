#include <iostream>

#include "Proc/ProcManager.h"
#include "Application.h"

int main()
{
	//Proc::GetProcessList();

	Application* app = new Application();

	app->Run();

	delete app;
}