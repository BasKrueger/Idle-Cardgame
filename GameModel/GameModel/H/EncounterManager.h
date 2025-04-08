#pragma once
#include "H/BaseEncounter.h"
#include "H/Json.hpp"
#include <memory>

class Player;

class EncounterManager
{
public:
	~EncounterManager();
	EncounterManager(Player* pPlayer);
	void Tick(Player* pPlayer);

private:
	int encounterIndex;

	BaseEncounter* pCurrentEncounter;

	void BeginNextEncounter(Player* pPlayer);
	BaseEncounter* GenerateNextEncounter(Player* pPlayer);

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	void SetSave(Player* pPlayer, json::JSON save);

#pragma endregion
};

