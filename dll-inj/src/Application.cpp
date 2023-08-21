#include "Application.h"

// syntax ???
Application* Application::s_Instance = nullptr;

Application::Application()
{
	if (!s_Instance)
	{
		s_Instance = this;
	}

	int success = glfwInit();
	m_Window = glfwCreateWindow(1280, 720, "dll-inj", nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable vsync

	m_ImGuiManager = new ImGuiManager();
}

Application::~Application()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Application::Start()
{
	while (m_Running) 
	{
		// ImGuiManager start frames
	}
}


void Application::End()
{
	m_Running = false;
}