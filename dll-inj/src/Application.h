#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

#include "Proc/Proc.h"
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
	std::shared_ptr<ProcModulesChunk> m_ProcModulesChunk;
	bool m_Running = false;

	static Application& GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance == new Application("dll-inj", 1280, 720);

		return *s_Instance;
	}

	static void FreeInstance()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	GLFWwindow* GetWindow() { return m_Window; }
	WindowData GetWindowData() { return m_WindowData; }
	std::vector<std::thread>& GetWorkerPool() { return m_Workers; }
	void Run();

	std::shared_ptr<ProcChunk> GetProcChunk() { return m_ProcChunk; }
	std::shared_ptr<ProcModulesChunk> GetProcModulesChunk() { return m_ProcModulesChunk; }

private:
	Application(std::string title, unsigned int width, unsigned int height);
	~Application();

	static Application* s_Instance;
	GLFWwindow* m_Window;
	WindowData m_WindowData;
	ImGuiManager* m_ImGuiManager;
	// TODO revision: thread pool implementation for thread reuse
	std::vector<std::thread> m_Workers;
};

static void RefreshProcesses();
