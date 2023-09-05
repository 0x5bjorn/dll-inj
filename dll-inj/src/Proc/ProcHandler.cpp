#include "ProcHandler.h"

void Proc::GetProcesses(std::vector<ProcInfo>& processes)
{
    processes.clear();

    HANDLE hProcessesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessesSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hProcessesSnap, &procEntry))
        {
            do {
                std::wstring procNameWstring(procEntry.szExeFile);
                using convert_type = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_type, wchar_t> converter;
                std::string procName = converter.to_bytes(procNameWstring);

                processes.emplace_back(procName, procEntry.th32ProcessID, procEntry.th32ParentProcessID);
                //_tprintf(TEXT("\n\n====================================================="));
                //_tprintf(TEXT("\nPROCESS NAME:  %s"), procEntry.szExeFile);
                //_tprintf(TEXT("\n-------------------------------------------------------"));
                //_tprintf(TEXT("\n  Process ID        = %d"), procEntry.th32ProcessID);
                //_tprintf(TEXT("\n  Thread count      = %d"), procEntry.cntThreads);
                //_tprintf(TEXT("\n  Parent process ID = %d"), procEntry.th32ParentProcessID);
                //_tprintf(TEXT("\n  Priority base     = %d"), procEntry.pcPriClassBase);
                //_tprintf(TEXT("\n  Usage     = %d"), procEntry.cntUsage);
            } while (Process32Next(hProcessesSnap, &procEntry));
        }
    }

    CloseHandle(hProcessesSnap);
}

void Proc::GetProcessModules(unsigned long pid)
{
    HANDLE hProcessModulesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

    if (hProcessModulesSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hProcessModulesSnap, &modEntry))
        {
            do {
                // Fill Struct 
                //_tprintf(TEXT("\n\n     MODULE NAME:     %s"), modEntry.szModule);
                //_tprintf(TEXT("\n     Executable     = %s"), modEntry.szExePath);
                //_tprintf(TEXT("\n     Process ID     = %d"), modEntry.th32ProcessID);
                //_tprintf(TEXT("\n     Ref count (g)  = %d"), modEntry.GlblcntUsage);
                //_tprintf(TEXT("\n     Ref count (p)  = %d"), modEntry.ProccntUsage);
                //_tprintf(TEXT("\n     Base address   = 0x%08X"), (DWORD)modEntry.modBaseAddr);
                //_tprintf(TEXT("\n     Base size      = %d"), modEntry.modBaseSize);
            } while (Module32Next(hProcessModulesSnap, &modEntry));
        }
    }

    CloseHandle(hProcessModulesSnap);
}
