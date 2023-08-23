#pragma once

#include <string>

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