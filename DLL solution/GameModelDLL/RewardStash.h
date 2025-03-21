#pragma once
#include "Reward.h"
#include <Vector>
#include "Json.hpp"

class Player;

class RewardStash
{
public:
	static void Initialize(Player* pPlayer);
	static void UnlockReward(int tier);
	static json::JSON* GetState();
	
	static void ClaimCardReward(int rewardID, int cardID, std::unique_ptr<Player>* pPlayer);
	static void ClaimBonusReward(int rewardID, std::unique_ptr<Player>* pPlayer);

private:
	static void CacheTier(int tier);

	static std::vector<Reward*> unclaimedRewards;
	static std::vector<std::vector<Reward*>> cachedRewards;
	static Player* pPlayer;

	static json::JSON* state;
};

