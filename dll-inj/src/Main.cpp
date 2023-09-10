#include <iostream>

#include "Application.h"

int main()
{
	Application& app = Application::GetInstance();
	app.Run();
	Application::FreeInstance();
}