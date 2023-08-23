#pragma once

#include <string>

struct ProcInfo
{
	std::string m_Name;
	int m_ProcessId;
	int m_ParentProcessId;

	ProcInfo(std::string processName, int processId, int parentProcessId)
		: m_Name(processName), m_ProcessId(processId), m_ParentProcessId(parentProcessId)
	{ }
};