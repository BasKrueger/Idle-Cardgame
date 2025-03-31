#include "pch.h"
#include "LogSubEntry.h"

json::JSON* LogSubEntry::GetState()
{
	if (state == nullptr) state = std::make_unique<json::JSON>();

	(*state)["ID"] = ID;
	(*state)["content"] = content->Format();

	return state.get();
}