#pragma once
#include "BaseEncounter.h"
#include <memory>
#include "Json.hpp"

class Player;

class EncounterManager
{
public:
	~EncounterManager();
	EncounterManager(std::unique_ptr<Player>* pPlayer);
	void Tick();

private:
	int encounterIndex;

	BaseEncounter* pCurrentEncounter;
	std::unique_ptr<Player>* pPlayer;

	void BeginNextEncounter();
	BaseEncounter* GenerateNextEncounter();

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	void SetSave(std::unique_ptr<Player>* pPlayer, json::JSON save);

#pragma endregion
};

