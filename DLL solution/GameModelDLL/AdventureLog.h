#include "LocalizedString.h"
#include <vector>
#include <iostream>
#include "Json.hpp"
#include "LogSubEntry.h"
#include "LogEntry.h"
#include <Array>

class AdventureLog 
{
public:
	static bool AddLog(LocalizedString*& log, std::string key, LocalizedString::TABLE table = LocalizedString::TABLE::ADVENTURE_LOG);
	static void BeginNewSection();
	static void Reset();
	static void SetLogRecordingActive(bool isEnabled);

private:
	static std::array<LogEntry*, 10> entries;
	static bool enabled;

#pragma region state/save/load
public:
	static json::JSON* GetState();
private:
	static json::JSON* state;
#pragma endregion
};