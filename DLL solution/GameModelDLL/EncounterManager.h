#pragma once
#include "BaseEncounter.h"
#include <memory>
#include "Json.hpp"

class Character;

class EncounterManager
{
public:
	~EncounterManager();
	EncounterManager(std::unique_ptr<Character>* pPlayer);
	void Tick();

	json::JSON* GetState();

private:
	BaseEncounter* pCurrentEncounter;
	int encounterIndex;
	std::unique_ptr<Character>* pPlayer;

	void BeginNextEncounter();
	BaseEncounter* GenerateNextEncounter();
};

