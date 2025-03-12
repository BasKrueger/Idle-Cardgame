#include "pch.h"
#include "RewardStash.h"
#include "GenericPool.h"

std::vector<Reward*> RewardStash::unclaimedRewards;
std::vector<std::vector<Reward*>> RewardStash::cachedRewards;
Player* RewardStash::pPlayer;
json::JSON* RewardStash::state;


void RewardStash::Initialize(Player* player)
{
	unclaimedRewards.clear();
	pPlayer = player;

	for(int i = 0;i < 3;i++)
	{
		std::vector<Reward*> v;
		cachedRewards.push_back(v);
		CacheTier(i);
	}

	UnlockReward(0);
	UnlockReward(0);
	UnlockReward(0);
	UnlockReward(0);
}

void RewardStash::UnlockReward(int tier)
{
	if(cachedRewards[tier].size() == 0)
	{
		CacheTier(tier);
	}

	unclaimedRewards.push_back(cachedRewards[tier].back());
	cachedRewards[tier].pop_back();
}

json::JSON* RewardStash::GetState()
{
	if (state == nullptr) state = new json::JSON();

	(*state)["earnedRewards"] = json::Array();
	for(auto& reward : unclaimedRewards)
	{
		(*state)["earnedRewards"].append(*reward->GetState());
	}

	return state;
}

void RewardStash::ClaimReward(int rewardID, int cardID, std::unique_ptr<Player>* pPlayer)
{
	int rewardIndex = -1;
	Reward* reward = nullptr;
	for(int i = 0;i < unclaimedRewards.size();i++)
	{
		if (unclaimedRewards[i]->id != rewardID) continue;
		reward = unclaimedRewards[i];
		rewardIndex = i;
		break;
	}

	if (reward == nullptr) return;

	if(reward->ClaimReward(cardID, pPlayer))
	{
		unclaimedRewards.erase(unclaimedRewards.begin() + rewardIndex);
	}
}

void RewardStash::CacheTier(int tier)
{
	for (int i = 0; i < 100; i++)
	{
		if (cachedRewards[tier].size() > 0) continue;

		auto reward = GenericPool<Reward>().GetInstance();
		reward->RandomizeReward(tier, pPlayer);

		cachedRewards[tier].push_back(reward);
	}
}
