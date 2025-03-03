#include "pch.h"
#include "AdventureLog.h"
#include "GenericPool.h"

std::array<LogEntry*, 10> AdventureLog::entries;
json::JSON* AdventureLog::state;
bool AdventureLog::enabled = true;

json::JSON* LogSubEntry::GetState()
{
	if (state == nullptr) state = std::make_unique<json::JSON>();

	(*state)["ID"] = ID;
	(*state)["content"] = content->Format();

	return state.get();
}

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
	for(auto& entry : subEntries)
	{
		entry->content->UnbindAll();
		entry->content->SetKey("Pool Error");

		GenericPool<LocalizedString>().ReturnInstance(entry->content);
		GenericPool<LogSubEntry>().ReturnInstance(entry);

		entry = 0;
	}

	subEntries.clear();
}

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

bool AdventureLog::AddLog(LocalizedString*& log, std::string key, LocalizedString::TABLE table)
{
	if (!enabled) return false;

	LogEntry* entry = 0;

	if (entries[0] == nullptr)
	{
		entries[0] = GenericPool<LogEntry>().GetInstance();
		entry = entries[0];
	}
	else
	{
		for (int i = entries.size() - 1; i >= 0; i--)
		{
			if (entries[i] != nullptr)
			{
				entry = entries[i];
				break;
			}
		}
	}

	if (entry == 0)
	{
		std::cout << "this shouldnt happen";
		return false;
	}

	log = entry->AddLog(key, table);
	return true;
}

void AdventureLog::BeginNewSection()
{
	if (!enabled) return;

	for(int i = 0; i < entries.size(); i++)
	{
		if(entries[i] == nullptr)
		{
			entries[i] = GenericPool<LogEntry>().GetInstance();
			return;
		}
	}
	
	entries[0]->Reset();
	GenericPool<LogEntry>().ReturnInstance(entries[0]);
	
	for (int i = 1; i < entries.size(); i++)
	{
		entries[i - 1] = entries[i];
	}
	
	entries[entries.size() - 1] = GenericPool<LogEntry>().GetInstance();
}

void AdventureLog::Reset()
{
	for(auto& entry : entries)
	{
		if (entry == nullptr) continue;

		entry->Reset();
		GenericPool<LogEntry>().ReturnInstance(entry);

		entry = nullptr;
	}
}

void AdventureLog::SetLogRecordingActive(bool isEnabled)
{
	enabled = isEnabled;
}

json::JSON* AdventureLog::GetState()
{
	if (state == nullptr) state = new json::JSON();

	(*state)["entries"] = json::Array();
	for(auto& entry : entries)
	{
		if (entry == nullptr) continue;

		(*state)["entries"].append(*entry->GetState());
	}

	return state;
}

