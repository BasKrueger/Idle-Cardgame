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

	json::JSON* GetState();

private:
	BaseEncounter* pCurrentEncounter;
	int encounterIndex;
	std::unique_ptr<Player>* pPlayer;

	void BeginNextEncounter();
	BaseEncounter* GenerateNextEncounter();
};

