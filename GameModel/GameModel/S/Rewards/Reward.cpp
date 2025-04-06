#include "H/Reward.h"
#include "H/CardPool.h"
#include "H/AttackCard.h"
#include "H/FastAttackCard.h"
#include "H/SlowAttackCard.h"

void Reward::RandomizeReward(int tier, Player* pPlayer)
{
	switch (tier) 
	{
		case 0:
			xp = 10;
			gold = 20;
			break;
		case 1:
			xp = 15;
			gold = 30;
			break;
		case 2:
			xp = 30;
			gold = 50;
			break;
	}

	std::array<int, MAX_REWARD_ID> possibleCards;
	for(int i = 0;i < cards.size();i++)
	{
		SetRandomCardReward(tier, i, pPlayer, &possibleCards);
	}
}

bool Reward::Claim(int cardID, Player* pPlayer)
{
	if (!ClaimCardReward(cardID, pPlayer)) return false;
	if (!ClaimBonusReward(pPlayer)) return false;

	return true;
}

bool Reward::ClaimCardReward(int cardID, Player* pPlayer)
{
	BaseCard* claimedCard = nullptr;

	for (auto& card : cards)
	{
		if (card == nullptr) continue;
		if (card->id != cardID) continue;
		
		pPlayer->AddToCollection(card);
		claimedCard = card;
		break;
	}
	
	if (claimedCard == nullptr) return false;

	for(int i = 0;i < cards.size(); i++)
	{
		if (cards[i] == nullptr) continue;
		if (cards[i] != claimedCard) cards[i]->ReturnToPool();

		cards[i] = nullptr;
	}

	return true;
}

bool Reward::ClaimBonusReward(Player* pPlayer)
{
	pPlayer->xp += xp;
	pPlayer->gold += gold;

	xp = 0;
	gold = 0;

	return true;
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

	cards[slot] = CardPool().GetInstance(pPlayer, rng);
	cards[slot]->UnRegister();
}

#pragma region state/save/load
json::JSON* Reward::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["gold"] = gold;
	(*state)["xp"] = xp;
	(*state)["cards"] = json::Array();
	(*state)["ID"] = id;

	for (auto& card : cards)
	{
		if (card == nullptr) continue;
		(*state)["cards"].append(*card->GetState());
	}

	return state;
}

json::JSON Reward::GetSave()
{
	json::JSON save;

	(save)["gold"] = gold;
	(save)["xp"] = xp;
	(save)["cards"] = json::Array();
	for (auto& card : cards)
	{
		if (card == nullptr) continue;
		(save)["cards"].append(card->GetSave());
	}

	return save;
}

Reward* Reward::LoadSave(Character* owner, json::JSON save)
{
	auto reward = new Reward();

	reward->gold = save["gold"].ToInt();
	reward->xp = save["xp"].ToInt();

	for (int i = 0; i < reward->cards.size(); i++)
	{
		if (reward->cards[i] == nullptr) continue;
		reward->cards[i]->ReturnToPool();
		reward->cards[i] = nullptr;
	}

	int iterator = -1;
	for (auto& card : save["cards"].ArrayRange())
	{
		iterator++;
		reward->cards[iterator] = BaseCard::LoadSave(owner, card);
	}

	return reward;
}

#pragma endregion