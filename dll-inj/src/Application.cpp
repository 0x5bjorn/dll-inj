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
	delete m_ImGuiManager;
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Application::Run()
{
	while (m_Running && !glfwWindowShouldClose(m_Window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		m_ImGuiManager->StartFrame();

		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::End();
		}

		m_ImGuiManager->EndFrame();

		glfwSwapBuffers(m_Window);
	}
}


void Application::Close()
{
	m_Running = false;
}