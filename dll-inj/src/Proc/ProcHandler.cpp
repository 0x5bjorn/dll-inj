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
                std::string procName = ConvertWcharToString(procEntry.szExeFile);
                processes.emplace_back(procName, procEntry.th32ProcessID, procEntry.th32ParentProcessID);
            } while (Process32Next(hProcessesSnap, &procEntry));
        }
    }

    CloseHandle(hProcessesSnap);
}

void Proc::GetProcessModules(unsigned long pid, std::vector<ProcModuleInfo>& procModules)
{
    procModules.clear();

    HANDLE hProcessModulesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hProcessModulesSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hProcessModulesSnap, &modEntry))
        {
            do {

                std::string modName = ConvertWcharToString(modEntry.szModule);
                std::string modPath = ConvertWcharToString(modEntry.szExePath);
                procModules.emplace_back(modEntry.th32ProcessID, modName, modPath, modEntry.modBaseSize);
            } while (Module32Next(hProcessModulesSnap, &modEntry));
        }
    }

    CloseHandle(hProcessModulesSnap);
}

