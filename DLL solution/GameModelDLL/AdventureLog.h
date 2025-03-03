#include "LocalizedString.h"
#include <vector>
#include <iostream>
#include "Json.hpp"
#include <vector>
#include <Array>

class LogSubEntry 
{
public:
	int ID = reinterpret_cast<int>(this);
	LocalizedString* content;

	json::JSON* GetState();

private:
	std::unique_ptr<json::JSON> state;
};

class LogEntry
{
public:
	LocalizedString* AddLog(std::string key, LocalizedString::TABLE table);
	void Reset();
	
	int id = reinterpret_cast<int>(this);
	json::JSON* GetState();

private:
	std::vector<LogSubEntry*> subEntries;
	std::unique_ptr<json::JSON> state;
};

class AdventureLog 
{
public:
	static bool AddLog(LocalizedString*& log, std::string key, LocalizedString::TABLE table = LocalizedString::TABLE::ADVENTURE_LOG);
	static void BeginNewSection();
	static void Reset();
	static void SetLogRecordingActive(bool isEnabled);

	static json::JSON* GetState();

private:
	static std::array<LogEntry*, 10> entries;
	static json::JSON* state;
	static bool enabled;

};