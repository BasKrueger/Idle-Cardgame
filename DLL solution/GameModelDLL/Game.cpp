#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "InteractionManager.h"
#include <mutex>
#include "Json.hpp"
#include "AdventureLog.h"
#include "RewardStash.h"
#include <chrono>

bool Game::stateGenerationEnabled = true;
std::unique_ptr<EncounterManager> Game::pEncounters; 
std::unique_ptr<Player> Game::pPlayer;
std::vector<std::string> Game::cachedStates;

void Game::Initialize()
{
	stateGenerationEnabled = false;

	if (pEncounters.get() != nullptr) {
		delete pEncounters.get();
		pEncounters.release();
	}
	if (pPlayer.get() != nullptr) {
		delete pPlayer.get();
		pPlayer.release();
	}

	pPlayer = std::make_unique<Player>();
	pPlayer->Initialize();
	pEncounters = std::make_unique<EncounterManager>(&pPlayer);
	
	RewardStash::Initialize(pPlayer.get());

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
	return pPlayer->SwitchCards(collectionID, deckID);
}

#define min(a, b) ((a) < (b) ? (a) : (b));
void Game::CaptureGameState()
{
	if (!stateGenerationEnabled) { return; }
	
	json::JSON j;

	j["player"] = *pPlayer->GetState();
	j["encounter"] = *pEncounters->GetState();
	j["log"] = *AdventureLog::GetState();
	j["rewards"] = *RewardStash::GetState();

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

void Game::ClaimCardReward(int rewardID, int cardID)
{
	RewardStash::ClaimCardReward(rewardID, cardID, &pPlayer);
}

void Game::ClaimBonusReward(int rewardID)
{
	RewardStash::ClaimBonusReward(rewardID, &pPlayer);
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
