#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(std::string title, unsigned int width, unsigned int height)
{
	m_WindowData.Title = title;
	m_WindowData.Width = width;
	m_WindowData.Height = height;

	if (!s_Instance)
		s_Instance = this;

	m_Running = true;
	int success = glfwInit();
	m_Window = glfwCreateWindow((int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable vsync

	m_ImGuiManager = new ImGuiManager();

	m_ProcChunk = std::make_shared<ProcChunk>();
	m_ProcModulesChunk = std::make_shared<ProcModulesChunk>();
	m_Workers.push_back(std::thread(&RefreshProcesses));
}

Application::~Application()
{
	m_Running = false;

	delete m_ImGuiManager;

	for (std::thread& worker : m_Workers)
		worker.join();

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

void RefreshProcesses()
{
	Application& app = Application::GetInstance();

	std::cout << "RefreshProcesses Worker started!" << std::endl;
	while (app.m_Running)
	{
		app.m_ProcChunk->UpdateProcesses();
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	std::cout << "RefreshProcesses Worker finished!" << std::endl;
}
