#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "InteractionManager.h"
#include <mutex>
#include "Json.hpp"
#include "AdventureLog.h"

bool Game::stateGenerationEnabled = true;
std::unique_ptr<EncounterManager> Game::pEncounters; 
std::unique_ptr<Character> Game::pPlayer;
std::vector<std::string> Game::cachedStates;

void Game::Initialize()
{
	stateGenerationEnabled = false;

	if (pEncounters != 0) {
		pEncounters.release();
	}
	if (pPlayer != 0) {
		pPlayer.release();
	}

	InteractionManager::Initialize();

	pPlayer = std::make_unique<Player>();
	pPlayer->Initialize();

	pEncounters = std::make_unique<EncounterManager>(&pPlayer);

	stateGenerationEnabled = true;
}

void Game::Tick()
{
	pEncounters->Tick();
	CaptureGameState();
}

float Game::Skip(float seconds)
{
	stateGenerationEnabled = false;

	auto startTime = std::chrono::system_clock::now();

	for(int i =0; i < seconds; i++)
	{
		Tick();
	}

	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsed_seconds = endTime - startTime;
	if(elapsed_seconds.count() > 1)
	{
		Skip(elapsed_seconds.count());
	}

	stateGenerationEnabled = true;

	return elapsed_seconds.count();
}

int Game::SwapCards(int collectionID, int deckID)
{
	return static_cast<Player*>(pPlayer.get())->SwitchCards(collectionID, deckID);
}

#define min(a, b) ((a) < (b) ? (a) : (b));
void Game::CaptureGameState()
{
	if (!stateGenerationEnabled) { return; }
	
	json::JSON j;

	j["player"] = *static_cast<Player*>(pPlayer.get())->GetState();
	j["encounter"] = *pEncounters->GetState();
	j["log"] = *AdventureLog::GetState();

	cachedStates.push_back(j.dump());
}

bool Game::PopCachedGameState(char* str, int strlen)
{
	if (cachedStates.size() == 0) return false;

	std::string result = cachedStates[0];
	cachedStates.erase(cachedStates.begin());

	result = result.substr(0, strlen);
	std::copy(result.begin(), result.end(), str);

	auto l = min(strlen - 1, (int)result.size());
	str[l] = 0;

	return true;
}

void Game::ClearCachedStates()
{
	cachedStates.clear();
}

void Game::SetSaveState(char* str, int strlen)
{
}

void Game::GetSaveState(char* str, int strlen)
{
}

void Game::Dump()
{
	CaptureGameState();
	std::cout << cachedStates.back() << "\n";
}
