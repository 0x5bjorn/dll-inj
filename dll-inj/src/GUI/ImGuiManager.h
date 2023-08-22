#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void StartFrame();
	void EndFrame();

private:
	// Process info data
};