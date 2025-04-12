#include "pch.h" 
#include "GameModelLib.h"
#include "H/Game.h"
#include "H/Localizer.h"

Game* pGame;

void Initialize()
{
	if (pGame != 0) 
	{
		delete pGame;
		pGame = 0;
	}

	pGame->Initialize();
}

void Tick()
{
	pGame->Tick();
}

void Skip(float seconds)
{
	pGame->Skip(seconds);
}

void SwapCards(int ID1, int ID2)
{
	return pGame->SwapCards(ID1, ID2);
}

void GenerateGameState()
{
	pGame->CaptureGameState();
}

bool PopCachedGameState(char* str, int strlen)
{
	return pGame->PopCachedGameState(str, strlen);
}

void ClearCachedStates()
{
	pGame->ClearCachedStates();
}

void SetLanguage(char* language, char* cardTable, char* logTable, char* characterTable)
{
	Localizer::SetLanguage(language, cardTable, logTable, characterTable);
}

void GetSaveState(char* str, int strlen)
{
	return pGame->GetSaveState(str, strlen);
}

void SetSaveState(char* str)
{
	pGame->SetSaveState(str);
}

void ClaimReward(int rewardID, int cardID)
{
	pGame->ClaimReward(rewardID, cardID);
}

void Dump() 
{
	pGame->Dump();
}
