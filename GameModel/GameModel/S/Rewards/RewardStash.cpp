#include "h/RewardStash.h"
#include "H/GenericPool.h"

std::array<Reward*, 99> RewardStash::unclaimedRewards;
std::array<std::array<Reward*, 30>, 3> RewardStash::cachedRewards;
Player* RewardStash::pPlayer;

void RewardStash::Initialize(Player* player)
{
	pPlayer = player;

	for(int i = 0;i < unclaimedRewards.size();i++)
	{
		if (unclaimedRewards[i] == nullptr) continue;
		GenericPool<Reward>().ReturnInstance(unclaimedRewards[i]);
		unclaimedRewards[i] = nullptr;
	}

	GenericPool<Reward*>().PrePool(100);
	
	RefillCacheTier(0);
	RefillCacheTier(1);
	RefillCacheTier(2);

	//Testing only
	UnlockReward(0);
	UnlockReward(0);
	UnlockReward(0);
	UnlockReward(0);
}

void RewardStash::UnlockReward(int tier)
{
	int emptyRewardSlot = -1;

	for (int i = 0; i < unclaimedRewards.size(); i++) 
	{
		if (unclaimedRewards[i] != nullptr) continue;
		emptyRewardSlot = i;
		break;
	}

	if (emptyRewardSlot == -1) return;

	Reward* reward = nullptr;
	for (int i = 0; i < cachedRewards[tier].size(); i++) 
	{
		if (cachedRewards[tier][i] == nullptr) continue;
		reward = cachedRewards[tier][i];
		cachedRewards[tier][i] = nullptr;
		break;
	}

	if(reward == nullptr)
	{
		RefillCacheTier(tier);
		UnlockReward(tier);
	}

	unclaimedRewards[emptyRewardSlot] = reward;
}

void RewardStash::ClaimReward(int rewardID, int cardID, Player* pPlayer)
{
	Reward* reward = nullptr;
	int rewardIndex = -1;
	for (int i = 0; i < unclaimedRewards.size(); i++)
	{
		if (unclaimedRewards[i] == nullptr) continue;
		if (unclaimedRewards[i]->id != rewardID) continue;

		reward = unclaimedRewards[i];
		rewardIndex = i;
		break;
	}

	if (reward == nullptr) return;
	if (reward->Claim(cardID, pPlayer)) 
	{
		GenericPool<Reward>().ReturnInstance(reward);
		unclaimedRewards[rewardIndex] = nullptr;
	}
}

void RewardStash::RefillCacheTier(int tier)
{
	for (int i = 0; i < cachedRewards[tier].size(); i++)
	{
		if (cachedRewards[tier][i] != nullptr) continue;

		auto reward = GenericPool<Reward>().GetInstance();
		reward->RandomizeReward(tier, pPlayer);

		cachedRewards[tier][i] = reward;
	}
} 

#pragma region state/save/load
json::JSON* RewardStash::state;

json::JSON* RewardStash::GetState()
{
	if (state == nullptr) state = new json::JSON();

	int rewardCount = 0;
	(*state)["earnedRewards"] = json::Array();
	for (auto& reward : unclaimedRewards)
	{
		if (reward == nullptr) continue;
		rewardCount++;

		if ((*state)["earnedRewards"].size() > 5) continue;
		(*state)["earnedRewards"].append(*reward->GetState());
	}

	(*state)["rewardCount"] = rewardCount;

	return state;
}

json::JSON RewardStash::GetSave()
{
	json::JSON save;

	save["earnedRewards"] = json::Array();
	for (auto& reward : unclaimedRewards)
	{
		if (reward == nullptr) continue;
		save["earnedRewards"].append(reward->GetSave());
	}

	return save;
}

void RewardStash::SetSave(Character* owner, json::JSON save)
{
	for (int i = 0; i < unclaimedRewards.size(); i++)
	{
		if (unclaimedRewards[i] == nullptr) continue;
		delete unclaimedRewards[i];
	}

	for (int i = 0; i < unclaimedRewards.size(); i++)
	{
		if (unclaimedRewards[i] == nullptr) continue;
		GenericPool<Reward>().ReturnInstance(unclaimedRewards[i]);
		unclaimedRewards[i] = nullptr;
	}

	int iterator = 0;
	for (auto& reward : save["earnedRewards"].ArrayRange())
	{
		unclaimedRewards[iterator] = Reward::LoadSave(owner, reward);
		iterator++;
	}

	GenericPool<Reward>().PrePool(100);
}

#pragma endregion
