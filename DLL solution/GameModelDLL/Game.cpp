#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "InteractionManager.h"
#include <mutex>
#include "Json.hpp"
#include "AdventureLog.h"
#include "RewardStash.h"
#include <chrono>

#define min(a, b) ((a) < (b) ? (a) : (b));

bool Game::stateGenerationEnabled = true;
std::unique_ptr<EncounterManager> Game::pEncounters; 
std::unique_ptr<Player> Game::pPlayer;

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
	auto startTime = std::chrono::system_clock::now();

	stateGenerationEnabled = false;
	for(int i =0; i < seconds; i++)
	{
		Tick();
	}
	stateGenerationEnabled = true;

	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsed_seconds = endTime - startTime;

	return elapsed_seconds.count();
}

int Game::SwapCards(int collectionID, int deckID)
{
	return pPlayer->SwitchCards(collectionID, deckID);
}

void Game::ClaimReward(int rewardID, int cardID)
{
	RewardStash::ClaimReward(rewardID, cardID, &pPlayer);
}

void Game::Dump()
{
	CaptureGameState();
	std::cout << cachedStates.back() << "\n";
}

#pragma region state/save/load
std::unique_ptr<json::JSON> Game::state;
std::vector<std::string> Game::cachedStates;

void Game::CaptureGameState()
{
	if (!stateGenerationEnabled) { return; }
	if (state == nullptr) state = std::make_unique<json::JSON>();

	(*state)["player"] = *pPlayer->GetState();
	(*state)["encounter"] = *pEncounters->GetState();
	(*state)["log"] = *AdventureLog::GetState();
	(*state)["rewards"] = *RewardStash::GetState();

	cachedStates.push_back(state->dump());
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

void Game::GetSaveState(char* str, int strlen)
{
	json::JSON save;

	save["player"] = pPlayer->GetSave();
	save["encounters"] = pEncounters->GetSave();
	save["rewards"] = RewardStash::GetSave();

	std::cout << save.dump() << "\n";

	if (strlen == 0) return;

	std::string result = save.dump();
	result = result.substr(0, strlen);
	std::copy(result.begin(), result.end(), str);
	auto l = min(strlen - 1, (int)result.size());
	str[l] = 0;
}

void Game::SetSaveState(char* str)
{
	ClearCachedStates();
	auto save = json::JSON::Load(str);

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
	pPlayer->SetSave(save["player"]);

	pEncounters = std::make_unique<EncounterManager>(&pPlayer);
	pEncounters->SetSave(&pPlayer, save["encounters"]);

	RewardStash::Initialize(pPlayer.get());
	RewardStash::SetSave(pPlayer.get(), save["rewards"]);

	stateGenerationEnabled = true;
	CaptureGameState();
}

#pragma endregion