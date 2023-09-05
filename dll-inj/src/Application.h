#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

#include "Proc/ProcHandler.h"
#include "GUI/ImGuiManager.h"

class Application
{
public:
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
	};
	std::shared_ptr<ProcChunk> m_ProcChunk;

	Application(std::string title, unsigned int width, unsigned int height);
	~Application();

	static Application& GetInstance() { return *s_Instance; }
	GLFWwindow* GetWindow() { return m_Window; }
	std::shared_ptr<ProcChunk> GetProcChunk() { return m_ProcChunk; }
	WindowData GetWindowData() { return m_WindowData; }
	void Run();

private:
	static Application* s_Instance;
	GLFWwindow* m_Window;
	WindowData m_WindowData;
	ImGuiManager* m_ImGuiManager;
	bool m_Running = false;
	std::thread m_Worker;
};
