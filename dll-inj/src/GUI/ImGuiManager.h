#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "../Proc/Proc.h"
#include "../OpenDialogBox/WinOpenDialogBox.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void StartFrame();
	void EndFrame();
	void DrawProcessTable();
	void DrawProcModulesTable();

private:
	bool m_ShowProcessModulesWindow = false;
};

static void RefreshProcModules(unsigned long&, bool&);
