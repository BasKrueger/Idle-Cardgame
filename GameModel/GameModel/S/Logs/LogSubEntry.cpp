#include "H/LogSubEntry.h"

json::JSON* LogSubEntry::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["ID"] = ID;
	(*state)["content"] = content->Format();

	return state;
}