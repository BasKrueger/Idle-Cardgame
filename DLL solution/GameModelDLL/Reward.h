#pragma once
#include <memory>
#include <array>
#include "BaseCard.h"
#include "Player.h"
#include "Json.hpp"
#include <Array>

#define MAX_REWARD_ID 3

class Reward
{
public:
	void RandomizeReward(int tier, Player* pPlayer);

	int gold;
	int xp;
	int id = reinterpret_cast<uint32_t>(this);
	std::array<BaseCard*, 3> cards;

	bool ClaimCardReward(int cardID, std::unique_ptr<Player>* pPlayer);
	bool ClaimBonusReward(std::unique_ptr<Player>* pPlayer);
	bool AllClaimed();

	json::JSON* GetState();

private:
	void SetRandomCardReward(int tier, int slot, Player* pPLayer, std::array<int, MAX_REWARD_ID>* excludeIDs);
	json::JSON* state;
};

