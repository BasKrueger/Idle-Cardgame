#include "H/CardPool.h"
#include "H/AttackCard.h"
#include "H/SlowAttackCard.h"
#include "H/FastAttackCard.h"

#define RegCard(CardClassName) case CardClassName::cardID: for (int i = 0; i < poolContent[CardClassName::cardID].size(); i++) \
{ if (poolContent[CardClassName::cardID][i] != nullptr) continue; poolContent[CardClassName::cardID][i] = new CardClassName();} break;

std::map<int, std::array<BaseCard*, 50>> CardPool::poolContent;

void CardPool::ReFillPool(int cardID)
{
	switch (cardID)
	{
		RegCard(AttackCard)
		RegCard(SlowAttackCard)
		RegCard(FastAttackCard)
	} 
}

BaseCard* CardPool::GetInstance(Character* owner, int cardID)
{
	BaseCard* result = nullptr;

	for (int i = 0;i < poolContent[cardID].size(); i++)
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

	result->Initialize();
	result->Register();
	result->pOwner = owner;

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