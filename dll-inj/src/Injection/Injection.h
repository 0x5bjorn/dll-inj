#pragma once

#include <windows.h>
#include <string>

namespace Injection {
	bool InjectDLL(std::string, unsigned long);
	bool CreateRemoteThreadWithInjectedDll(HANDLE&, LPVOID&);
}
