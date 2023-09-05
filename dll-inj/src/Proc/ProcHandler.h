#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <mutex>
#include <locale>
#include <codecvt>

struct ProcInfo
{
	std::string m_Name;
	int m_ProcessId;
	int m_ParentProcessId;

	ProcInfo(std::string processName, int processId, int parentProcessId)
		: m_Name(processName), m_ProcessId(processId), m_ParentProcessId(parentProcessId)
	{ }
};

struct ProcModuleInfo
{
	int m_ProcessId;
	std::string m_Name;
	std::string m_Path;
	int m_Size;

	ProcModuleInfo(int processId, std::string moduleName, std::string modulePath, int moduleSize)
		: m_ProcessId(processId), m_Name(moduleName), m_Path(modulePath), m_Size(moduleSize)
	{ }
};

namespace Proc {
	void GetProcesses(std::vector<ProcInfo>&);
	void GetProcessModules(unsigned long);
}

struct ProcChunk
{
	std::vector<ProcInfo> m_Processes;
	std::mutex m_Mutex;

	ProcChunk()
	{
		m_Processes.reserve(1024);
	}

	void UpdateProcesses()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		Proc::GetProcesses(m_Processes);
	}
};
