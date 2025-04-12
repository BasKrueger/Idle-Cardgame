#pragma once
#include "H/EncounterManager.h"
#include "H/Character.h"
#include <vector>

class Player;

class Game
{
public:
	void Initialize();
	void Tick();
	void Skip(float seconds);
	void SwapCards(int ID1, int ID2);
	void ClaimReward(int rewardID, int cardID);

	void Dump();

private:
	static EncounterManager* pEncounters;
	static Player* pPlayer;

#pragma region state/save/load
public:
	static void CaptureGameState();
	bool PopCachedGameState(char* str, int strlen);
	void ClearCachedStates();

	void GetSaveState(char* str, int strlen);
	void SetSaveState(char* str);

private:
	static json::JSON* state;
	static std::vector<std::string> cachedStates;
	
	static bool stateGenerationEnabled;
#pragma endregion
};

