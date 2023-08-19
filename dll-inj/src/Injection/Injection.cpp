#include "Injection.h"

bool Injection::InjectDLL(std::string dllPath, unsigned long pid) {
	unsigned int dllSize = dllPath.length() + 2;

	// Get remote process handle
	printf("PID: %i\n", pid);
	DWORD dwPID = DWORD(pid);

	HANDLE hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hRemoteProcess == nullptr) {
		printf("Error with OpenProcess()\n");
		return FALSE;
	}

	// Allocate memory buffer and copy/write dll in remote process
	LPVOID rpBuffer = VirtualAllocEx(hRemoteProcess, nullptr, dllSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (rpBuffer == nullptr) {
		printf("Error with VirtualAllocEx()\n");
		return FALSE;
	}

	if (!WriteProcessMemory(hRemoteProcess, rpBuffer, dllPath.c_str(), dllSize, nullptr)) {
		printf("Error with WriteProcessMemory()\n");
		return FALSE;
	}

	if (!CreateRemoteThreadWithInjectedDll(hRemoteProcess, rpBuffer)) {
		printf("Error with CreateRemoteThreadWithInjectedDll()\n");
		return FALSE;
	}

	CloseHandle(hRemoteProcess);
	hRemoteProcess = nullptr;
}

bool Injection::CreateRemoteThreadWithInjectedDll(HANDLE& hRemoteProcess, LPVOID& rpBuffer) {
	// Get LoadLibraryA() function address from Kernel32 library/module, 
	// required for CreateRemoteThread() to create remote thread which will load our dll
	HMODULE hKernel32 = GetModuleHandle(TEXT("Kernel32"));
	if (hKernel32 == nullptr) {
		printf("Error with GetModuleHandle()\n");
		return FALSE;
	}
	VOID* paLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

	// Create remote thread which will load our dll
	HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, nullptr, NULL, (LPTHREAD_START_ROUTINE)paLoadLibraryA, rpBuffer, NULL, nullptr);
	if (hRemoteThread == nullptr) {
		VirtualFreeEx(hRemoteProcess, rpBuffer, 0x0, MEM_RELEASE);
		rpBuffer = nullptr;
		CloseHandle(hRemoteProcess);
		hRemoteProcess = nullptr;

		printf("Error with CreateRemoteThread()\n");
		return FALSE;
	}

	CloseHandle(hKernel32);
	hKernel32 = nullptr;
}