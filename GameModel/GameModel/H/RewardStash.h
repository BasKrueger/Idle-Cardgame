#pragma once
#include "Reward.h"
#include <Vector>
#include <Array>
#include "Json.hpp"

class Player;

class RewardStash
{
public:
	static void Initialize(Player* pPlayer);
	static void UnlockReward(int tier);

	static void ClaimReward(int rewardID, int cardID, Player* pPlayer);

private:
	static Player* pPlayer;
	static std::array<Reward*, 99> unclaimedRewards;
	static std::array<std::array<Reward*, 30>, 3> cachedRewards;

	static void RefillCacheTier(int tier);

#pragma region state/save/load
public:
	static json::JSON* GetState();
	static json::JSON GetSave();
	static void SetSave(Character* owner, json::JSON);

private:
	static json::JSON* state;
#pragma endregion
};

