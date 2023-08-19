#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Proc/Proc.h"

int main()
{
	Proc::GetProcessList();
	std::cin.get();
}