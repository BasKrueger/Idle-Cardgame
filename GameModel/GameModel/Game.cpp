#include "H/Game.h"
#include "H/Player.h"
#include "H/InteractionManager.h"
#include "H/Json.hpp"
#include "H/AdventureLog.h"
#include "H/RewardStash.h"

#define min(a, b) ((a) < (b) ? (a) : (b));

bool Game::stateGenerationEnabled = true;
EncounterManager* Game::pEncounters = nullptr;
Player* Game::pPlayer = nullptr;

void Game::Initialize()
{
	stateGenerationEnabled = false;

	if (pEncounters != nullptr) {
		delete pEncounters;
		pEncounters = nullptr;
	}
	if (pPlayer != nullptr) {
		delete pPlayer;
		pPlayer = nullptr;
	}

	pPlayer = new Player;
	pPlayer->Initialize();
	pEncounters = new EncounterManager(pPlayer);
	
	RewardStash::Initialize(pPlayer);

	stateGenerationEnabled = true;
}

void Game::Tick()
{
	pEncounters->Tick();
	CaptureGameState();
}

void Game::Skip(float seconds)
{
	stateGenerationEnabled = false;
	for(int i =0; i < seconds; i++)
	{
		Tick();
	}
	stateGenerationEnabled = true;
}

int Game::SwapCards(int collectionID, int deckID)
{
	return pPlayer->SwitchCards(collectionID, deckID);
}

void Game::ClaimReward(int rewardID, int cardID)
{
	RewardStash::ClaimReward(rewardID, cardID, pPlayer);
}

void Game::Dump()
{
	CaptureGameState();
	std::cout << cachedStates.back() << "\n";
}

#pragma region state/save/load
json::JSON* Game::state;
std::vector<std::string> Game::cachedStates;

void Game::CaptureGameState()
{
	if (!stateGenerationEnabled) { return; }
	if (state == nullptr) state = new json::JSON;

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

	if (pEncounters != nullptr) {
		delete pEncounters;
		pEncounters = nullptr;
	}
	if (pPlayer != nullptr) {
		delete pPlayer;
		pPlayer = nullptr;
	}

	pPlayer = new Player;
	pPlayer->Initialize();
	pPlayer->SetSave(save["player"]);

	pEncounters = new EncounterManager(pPlayer);
	pEncounters->SetSave(pPlayer, save["encounters"]);

	RewardStash::Initialize(pPlayer);
	RewardStash::SetSave(pPlayer, save["rewards"]);

	stateGenerationEnabled = true;
	CaptureGameState();
}

#pragma endregion