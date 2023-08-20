#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

namespace Proc {
	void GetProcessList();
	void GetProcessModulesList(unsigned long);
}
