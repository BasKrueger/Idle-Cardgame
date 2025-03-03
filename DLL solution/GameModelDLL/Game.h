#pragma once
#include "pch.h"
#include "EncounterManager.h"
#include "Character.h"
#include <vector>

class Game
{
public:
	void Initialize();
	void Tick();
	float Skip(float seconds);
	int SwapCards(int collectionID, int deckID);
	static void CaptureGameState();

	bool PopCachedGameState(char* str, int strlen);
	void ClearCachedStates();

	void SetSaveState(char* str, int strlen);
	void GetSaveState(char* str, int strlen);

	void Dump();

private:

	static bool stateGenerationEnabled;

	static std::unique_ptr<EncounterManager> pEncounters;
	static std::unique_ptr<Character> pPlayer;
	static std::vector<std::string> cachedStates;
};

