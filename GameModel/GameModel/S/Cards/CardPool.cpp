#include "H/CardPool.h"
#include "H/AttackCard.h"
#include "H/SlowAttackCard.h"
#include "H/FastAttackCard.h"
#include "H/HealPotionCard.h"
#include "H/PowerPotionCard.h"
#include "H/DoubleStrikeCard.h"
#include "H/CrazyPotionCard.h"
#include "H/ChargedStrikeCard.h"
#include "H/SpikeShieldCard.h"
#include "H/HealingShieldCard.h"
#include "H/TieUpCard.h"
#include "H/CrossSwords.h"
#include "H/SwordHail.h"
#include "H/HarmlessPotion.h"
#include "H/BreakSword.h"
#include "H/BootsCard.h"
#include "H/ThrowSwordCard.h"
#include "H/MightySwordCard.h"
#include "H/WoundingStrikeCard.h"

#define RegCard(CardClassName) case CardClassName::cardID: for (int i = 0; i < poolContent[CardClassName::cardID].size(); i++) \
{ if (poolContent[CardClassName::cardID][i] != nullptr) continue; poolContent[CardClassName::cardID][i] = new CardClassName();} break;

std::map<int, std::array<BaseCard*, 10>> CardPool::poolContent;
int CardPool::maxCardID = 0;

void CardPool::Initialize()
{
	for (maxCardID = 0; ReFillPool(maxCardID); maxCardID++);
	maxCardID--;
}

bool CardPool::ReFillPool(int cardID)
{
	switch (cardID)
	{
		RegCard(AttackCard)
			RegCard(SlowAttackCard)
			RegCard(FastAttackCard)
			RegCard(HealPotionCard)
			RegCard(PowerPotionCard)
			RegCard(DoubleStrikeCard)
			RegCard(CrazyPotionCard)
			RegCard(ChargedStrikeCard)
			RegCard(SpikeShieldCard)
			RegCard(HealingShieldCard)
			RegCard(TieUpCard)
			RegCard(CrossSwords)
			RegCard(SwordHail)
			RegCard(HarmlessPotion)
			RegCard(BreakSword)
			RegCard(BootsCard)
			RegCard(ThrowSwordCard)
			RegCard(MightySwordCard)
			RegCard(WoundingStrikeCard)

	default:
		return false;
	}

	return true;
}

BaseCard* CardPool::GetInstance(Character* owner, int cardID)
{
	BaseCard* result = nullptr;

	for (int i = 0;i < poolContent[cardID].size() ; i++)
	{
		if (poolContent[cardID][i] == nullptr) continue;
		result = poolContent[cardID][i];
		poolContent[cardID][i] = nullptr;
		break;
	}

	if (result == nullptr) 
	{
		ReFillPool(cardID);
		return GetInstance(owner, cardID);
	}

	result->pOwner = owner;
	result->Register();
	result->Initialize();

	return result;
}

void CardPool::ReturnInstance(BaseCard* pInstance)
{
	pInstance->UnRegister();
	auto array = &poolContent[pInstance->cardID];

	for(int i = 0;i < array->size(); i++)
	{
		if ((*array)[i] != nullptr) continue;
		(*array)[i] = pInstance;
		return;
	}
}