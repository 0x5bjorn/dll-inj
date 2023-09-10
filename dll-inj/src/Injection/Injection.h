#pragma once

#include <windows.h>
#include <string>

namespace Injection
{
	bool InjectDLL(std::string, unsigned long);
}

static bool CreateRemoteThreadWithInjectedDll(HANDLE& hRemoteProcess, LPVOID& rpBuffer);
