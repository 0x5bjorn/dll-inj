#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "GUI/ImGuiManager.h"

class Application
{
public:
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
	};

	Application(std::string title, unsigned int width, unsigned int height);
	~Application();

	static Application& GetInstance() { return *s_Instance; }
	GLFWwindow* GetWindow() { return m_Window; };
	WindowData GetWindowData() { return m_WindowData; };
	void Run();

private:
	static Application* s_Instance;
	GLFWwindow* m_Window;
	ImGuiManager* m_ImGuiManager;
	WindowData m_WindowData;
};
