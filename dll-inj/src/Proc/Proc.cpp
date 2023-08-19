#include "Proc.h"

void Proc::GetProcessList() {
    HANDLE hProcessesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessesSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hProcessesSnap, &procEntry)) {
            do {
                // Fill Struct 
                _tprintf(TEXT("\n\n====================================================="));
                _tprintf(TEXT("\nPROCESS NAME:  %s"), procEntry.szExeFile);
                _tprintf(TEXT("\n-------------------------------------------------------"));
                _tprintf(TEXT("\n  Process ID        = 0x%08X"), procEntry.th32ProcessID);
                _tprintf(TEXT("\n  Thread count      = %d"), procEntry.cntThreads);
                _tprintf(TEXT("\n  Parent process ID = 0x%08X"), procEntry.th32ParentProcessID);
                _tprintf(TEXT("\n  Priority base     = %d"), procEntry.pcPriClassBase);
            } while (Process32Next(hProcessesSnap, &procEntry));
        }
    }

    CloseHandle(hProcessesSnap);
}

void Proc::GetProcessModulesList(unsigned long pid) {
    HANDLE hProcessModulesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

    if (hProcessModulesSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hProcessModulesSnap, &modEntry)) {
            do {
                // Fill Struct 
                _tprintf(TEXT("\n\n     MODULE NAME:     %s"), modEntry.szModule);
                _tprintf(TEXT("\n     Executable     = %s"), modEntry.szExePath);
                _tprintf(TEXT("\n     Process ID     = 0x%08X"), modEntry.th32ProcessID);
                _tprintf(TEXT("\n     Ref count (g)  = 0x%04X"), modEntry.GlblcntUsage);
                _tprintf(TEXT("\n     Ref count (p)  = 0x%04X"), modEntry.ProccntUsage);
                _tprintf(TEXT("\n     Base address   = 0x%08X"), (DWORD)modEntry.modBaseAddr);
                _tprintf(TEXT("\n     Base size      = %d"), modEntry.modBaseSize);
            } while (Module32Next(hProcessModulesSnap, &modEntry));
        }
    }

    CloseHandle(hProcessModulesSnap);
}
