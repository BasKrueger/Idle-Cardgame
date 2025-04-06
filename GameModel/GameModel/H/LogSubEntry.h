#pragma once
#include "LocalizedString.h"
#include "Json.hpp"
#include "H/IDManager.h"

class LogSubEntry
{
public:
	int ID = IDManager::NewID();
	LocalizedString* content;

#pragma region state/save/load
public:
	json::JSON* GetState();
private:
	json::JSON* state;
#pragma endregion
};