#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "GameModelLib.h"
#include "Game.h"
#include "Localizer.h"
#include "AdventureLog.h"

Game* pGame;

void Initialize()
{
	if (pGame != 0) 
	{
		delete pGame;
		pGame = 0;
	}

	AdventureLog::Reset();

	LocalizedString* log;
	AdventureLog::AddLog(log, "RunStart");

	pGame = new Game;
	pGame->Initialize();
}

void Tick()
{
	pGame->Tick();
}

float Skip(float seconds)
{
	AdventureLog::Reset();
	AdventureLog::SetLogRecordingActive(false);
	
	auto result = pGame->Skip(seconds);
	
	AdventureLog::SetLogRecordingActive(true);

	return result;
}

int SwapCards(int collectionID, int deckID)
{
	return pGame->SwapCards(collectionID, deckID);
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
	
}

void SetSaveState(char* str, int strlen)
{
	
}

void Dump() 
{
	pGame->Dump();
}
