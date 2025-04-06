#pragma once
#include <memory>
#include <array>
#include "BaseCard.h"
#include "Player.h"
#include "Json.hpp"
#include "H/IDManager.h"

#define MAX_REWARD_ID 3

class Reward
{
public:
	void RandomizeReward(int tier, Player* pPlayer);

	int gold;
	int xp;
	int id = IDManager::NewID();
	std::array<BaseCard*, 3> cards;

	bool Claim(int cardID, Player* pPlayer);

private:
	bool ClaimCardReward(int cardID, Player* pPlayer);
	bool ClaimBonusReward(Player* pPlayer);
	void SetRandomCardReward(int tier, int slot, Player* pPLayer, std::array<int, MAX_REWARD_ID>* excludeIDs);

#pragma region state/save/load
	json::JSON* state;

public:
	json::JSON* GetState();
	json::JSON GetSave();
	static Reward* LoadSave(Character* owner, json::JSON save);
#pragma endregion
};

