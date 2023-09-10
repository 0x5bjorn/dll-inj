#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <mutex>
#include <locale>

#include "../Utils.h"

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
	void GetProcessModules(unsigned long, std::vector<ProcModuleInfo>&);
}

struct ProcChunk
{
	std::vector<ProcInfo> m_Processes;
	std::mutex m_Mutex;

	ProcChunk()
	{
		m_Processes.reserve(512);
	}

	void UpdateProcesses()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		Proc::GetProcesses(m_Processes);
	}
};

struct ProcModulesChunk
{
	std::vector<ProcModuleInfo> m_ProcModules;
	std::mutex m_Mutex;

	ProcModulesChunk()
	{
		m_ProcModules.reserve(1024);
	}

	void UpdateProcModules(unsigned long pid)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		Proc::GetProcessModules(pid, m_ProcModules);
	}
};
