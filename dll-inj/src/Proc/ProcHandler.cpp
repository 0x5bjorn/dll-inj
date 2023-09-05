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
                using convert_type = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_type, wchar_t> converter;

                std::wstring modNameWstring(modEntry.szModule);
                std::string modName = converter.to_bytes(modNameWstring);

                std::wstring modPathWstring(modEntry.szExePath);
                std::string modPath = converter.to_bytes(modPathWstring);

                procModules.emplace_back(modEntry.th32ProcessID, modName, modPath, modEntry.modBaseSize);
            } while (Module32Next(hProcessModulesSnap, &modEntry));
        }
    }

    CloseHandle(hProcessModulesSnap);
}
