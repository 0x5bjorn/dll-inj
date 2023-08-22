#pragma once

#include <GLFW/glfw3.h>

#include "GUI/ImGuiManager.h"

class Application
{
public:

	static Application& GetInstance() { return *s_Instance; }
	GLFWwindow* GetWindow() { return m_Window; };
	void Run();
	void Close();

	Application();
	~Application();

private:
	static Application* s_Instance;
	GLFWwindow* m_Window;
	bool m_Running = true;
	ImGuiManager* m_ImGuiManager;
};
