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
	void Tick();

private:
	int encounterIndex;

	BaseEncounter* pCurrentEncounter;
	Player* pPlayer;

	void BeginNextEncounter();
	BaseEncounter* GenerateNextEncounter();

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	void SetSave(Player* pPlayer, json::JSON save);

#pragma endregion
};

