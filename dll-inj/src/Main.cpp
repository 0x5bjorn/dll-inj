#include <iostream>

#include "Application.h"

int main()
{
	Application* app = new Application("dll-inj", 1280, 720);

	app->Run();

	delete app;
}