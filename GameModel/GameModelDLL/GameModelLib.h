#pragma once

#ifdef GAMEMODEL_EXPORTS
#define GAMEMODEL_API __declspec(dllexport)
#else
#define GAMEMODEL_API __declspec(dllimport)
#endif

extern "C" 
{
	GAMEMODEL_API void Initialize();

	GAMEMODEL_API void Tick();

	GAMEMODEL_API void Skip(float seconds);

	GAMEMODEL_API void SwapCards(int ID1, int ID2);

	GAMEMODEL_API void GenerateGameState();

	GAMEMODEL_API bool PopCachedGameState(char* str, int strlen);

	GAMEMODEL_API void ClearCachedStates();

	GAMEMODEL_API void SetLanguage(char* language, char* cardTabl, char* logTable, char* characterTable);

	GAMEMODEL_API void GetSaveState(char* str, int strlen);

	GAMEMODEL_API void SetSaveState(char* str);

	GAMEMODEL_API void ClaimReward(int rewardID, int cardID);

	GAMEMODEL_API void Dump();
}

