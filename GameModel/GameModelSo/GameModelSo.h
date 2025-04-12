#pragma once

extern "C"
{
	void Initialize();

	void Tick();

	void Skip(float seconds);

	void SwapCards(int collectionID, int deckID);

	void GenerateGameState();

	bool PopCachedGameState(char* str, int strlen);

	void ClearCachedStates();

	void SetLanguage(char* language, char* cardTabl, char* logTable, char* characterTable);

	void GetSaveState(char* str, int strlen);

	void SetSaveState(char* str);

	void ClaimReward(int rewardID, int cardID);

	void Dump();
}

