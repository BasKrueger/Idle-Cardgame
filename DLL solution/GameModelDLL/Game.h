#pragma once
#include "pch.h"
#include "EncounterManager.h"
#include "Character.h"
#include <vector>

class Player;

class Game
{
public:
	void Initialize();
	void Tick();
	float Skip(float seconds);
	int SwapCards(int collectionID, int deckID);
	void ClaimReward(int rewardID, int cardID);

	void Dump();

private:
	static std::unique_ptr<EncounterManager> pEncounters;
	static std::unique_ptr<Player> pPlayer;

#pragma region state/save/load
public:
	static void CaptureGameState();
	bool PopCachedGameState(char* str, int strlen);
	void ClearCachedStates();

	void GetSaveState(char* str, int strlen);
	void SetSaveState(char* str);

private:
	static std::unique_ptr<json::JSON> state;
	static std::vector<std::string> cachedStates;
	
	static bool stateGenerationEnabled;
#pragma endregion
};

