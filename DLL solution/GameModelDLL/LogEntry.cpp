#include "pch.h"
#include "LogEntry.h"
#include "GenericPool.h"
#include "AdventureLog.h"

LocalizedString* LogEntry::AddLog(std::string key, LocalizedString::TABLE table)
{
	auto log = GenericPool<LocalizedString>().GetInstance();

	log->UnbindAll();
	log->SetKey(key);
	log->SetTable(table);

	auto sub = GenericPool<LogSubEntry>().GetInstance();
	sub->content = log;
	subEntries.push_back(sub);

	return log;
}

void LogEntry::Reset()
{
	for (auto& entry : subEntries)
	{
		entry->content->UnbindAll();
		entry->content->SetKey("Pool Error");

		GenericPool<LocalizedString>().ReturnInstance(entry->content);
		GenericPool<LogSubEntry>().ReturnInstance(entry);

		entry = 0;
	}

	GenericPool<LogSubEntry>().PrePool(200);

	subEntries.clear();
}

#pragma region state/save/load

json::JSON* LogEntry::GetState()
{
	if (state == nullptr) state = std::make_unique<json::JSON>();

	(*state)["ID"] = id;
	(*state)["subEntries"] = json::Array();

	for (auto& entry : subEntries)
	{
		(*state)["subEntries"].append(*entry->GetState());
	}

	return state.get();
}

#pragma endregion