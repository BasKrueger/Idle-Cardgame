#include "pch.h"
#include "Reward.h"
#include "CardPool.h"
#include "AttackCard.h"
#include "FastAttackCard.h"
#include "SlowAttackCard.h"

#define Reward(className) case className::rewardID: cards[slot] = CardPool<className>().GetInstance(pPlayer); break;

void Reward::RandomizeReward(int tier, Player* pPlayer)
{
	switch (tier) 
	{
		case 0:
			xp = 1;
			gold = 2;
			break;
		case 1:
			xp = 2;
			gold = 2;
			break;
		case 2:
			xp = 5;
			gold = 10;
			break;
	}

	std::array<int, MAX_REWARD_ID> possibleCards;
	for(int i = 0;i < cards.size();i++)
	{
		SetRandomCardReward(tier, i, pPlayer, &possibleCards);
	}
}

bool Reward::ClaimReward(int cardID, std::unique_ptr<Player>* pPlayer)
{
	for (auto& card : cards)
	{
		if (card->id != cardID) continue;
		
		pPlayer->get()->AddToCollection(card);
		pPlayer->get()->xp += xp;
		pPlayer->get()->gold += gold;

		return true;
	}

	return false;
}

json::JSON* Reward::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["gold"] = gold;
	(*state)["xp"] = xp;
	(*state)["cards"] = json::Array();
	(*state)["ID"] = id;

	for(auto& card : cards)
	{
		(*state)["cards"].append(*card->GetState());
	}

	return state;
}

void Reward::SetRandomCardReward(int tier, int slot, Player* pPlayer, std::array<int, MAX_REWARD_ID>* excludeIDs)
{
	if (cards[slot] != nullptr)
	{
		cards[slot]->ReturnToPool();
		cards[slot] = nullptr;
	}

	int rng = -1;
	bool duplicate = true;

	do
	{
		rng = rand() % MAX_REWARD_ID;
	} while ((*excludeIDs)[rng] == -1);
		
	(*excludeIDs)[rng] = -1;
	
	bool rerun = true;

	switch(rng)
	{
		Reward(AttackCard);
		Reward(FastAttackCard);
		Reward(SlowAttackCard);

	default:
		std::cout << "ERROR: REWARD ID MISSING!\n";
		break;
	}
	
	cards[slot]->UnRegister();
}
