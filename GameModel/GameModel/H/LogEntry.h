#pragma once
#include "LocalizedString.h"
#include "LogSubEntry.h"
#include "IDManager.h"

class LogEntry
{
public:
	int id = IDManager::NewID();

	LocalizedString* AddLog(std::string key, LocalizedString::TABLE table);
	void Reset();

private:
	std::vector<LogSubEntry*> subEntries;

#pragma region state/save/load
public:
	json::JSON* GetState();

private:
	json::JSON* state;
#pragma endregion
};