#include "pch.h"
#include "LocalizedStringLibrary.h"

json::JSON* LocalizedStringLibrary::state = 0;
std::vector<LocalizedString*> LocalizedStringLibrary::createdStrings;

LocalizedString* LocalizedStringLibrary::GetInstance(LocalizedString::TABLE table)
{
	auto* string = new LocalizedString(table);
	createdStrings.push_back(string);

	return string;
}

json::JSON* LocalizedStringLibrary::GetState()
{
	if (state == 0) state = new json::JSON();

	(*state)["values"] = json::Array();
	for (auto& value : createdStrings)
	{
		(*state)["values"].append(*value->GetState());
	}

	(*state)["Ids"] = json::Array();
	for (auto& value : createdStrings)
	{
		(*state)["Ids"].append((int)value->GetState());
	}

	return state;
}