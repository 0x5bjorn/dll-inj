#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(std::string title, unsigned int width, unsigned int height)
{
	m_WindowData.Title = title;
	m_WindowData.Width = width;
	m_WindowData.Height = height;

	if (!s_Instance)
	{
		s_Instance = this;
	}

	int success = glfwInit();
	m_Window = glfwCreateWindow((int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable vsync

	m_ImGuiManager = new ImGuiManager();
}

Application::~Application()
{
	delete m_ImGuiManager;
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Application::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		m_ImGuiManager->StartFrame();
		m_ImGuiManager->DrawTable();
		m_ImGuiManager->EndFrame();

		glfwSwapBuffers(m_Window);
	}
}
