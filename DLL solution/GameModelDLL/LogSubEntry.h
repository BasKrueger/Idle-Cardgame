#pragma once
#include "LocalizedString.h"
#include "Json.hpp"

class LogSubEntry
{
public:
	int ID = reinterpret_cast<int>(this);
	LocalizedString* content;

#pragma region state/save/load
public:
	json::JSON* GetState();
private:
	std::unique_ptr<json::JSON> state;
#pragma endregion
};