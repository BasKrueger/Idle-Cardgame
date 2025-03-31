#pragma once
#include "LocalizedString.h"
#include "LogSubEntry.h"

class LogEntry
{
public:
	int id = reinterpret_cast<int>(this);

	LocalizedString* AddLog(std::string key, LocalizedString::TABLE table);
	void Reset();

private:
	std::vector<LogSubEntry*> subEntries;

#pragma region state/save/load
public:
	json::JSON* GetState();

private:
	std::unique_ptr<json::JSON> state;
#pragma endregion
};