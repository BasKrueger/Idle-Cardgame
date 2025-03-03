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

	GAMEMODEL_API float Skip(float seconds);

	GAMEMODEL_API int SwapCards(int collectionID, int deckID);

	GAMEMODEL_API void GenerateGameState();

	GAMEMODEL_API bool PopCachedGameState(char* str, int strlen);

	GAMEMODEL_API void ClearCachedStates();

	GAMEMODEL_API void SetLanguage(char* language, char* cardTabl, char* logTable, char* characterTable);

	GAMEMODEL_API void GetSaveState(char* str, int strlen);

	GAMEMODEL_API void SetSaveState(char* str, int strlen);

	GAMEMODEL_API void Dump();
}

