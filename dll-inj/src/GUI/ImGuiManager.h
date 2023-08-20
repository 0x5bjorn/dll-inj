#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

class ImGuiManager {
public:
	ImGuiManager();
	~ImGuiManager();

	void StartFrame();
	void EndFrame();

private:
	GLFWwindow* m_Window;
	// Process info data
};