#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "../Proc/ProcHandler.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void StartFrame();
	void EndFrame();
	void DrawTable();

private:
	bool m_ShowProcessModulesWindow = false;
};

void RefreshProcModules(unsigned long pid, bool& showProcessModulesWindow);
