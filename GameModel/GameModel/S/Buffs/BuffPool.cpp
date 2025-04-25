#include "H/BuffPool.h"
#include "H/Buff.h"
#include "H/BaseCard.h"
#include "H/BleedBuff.h"

#define RegBuff(BuffClassName) case BuffClassName::buffID: for (int i = 0; i < poolContent[BuffClassName::buffID].size(); i++) \
{ if (poolContent[BuffClassName::buffID][i] != nullptr) continue; poolContent[BuffClassName::buffID][i] = new BuffClassName();} break;

std::map<int, std::array<Buff*, 10>> BuffPool::poolContent;

void BuffPool::Initialize()
{
	for (int i = 0; ReFillPool(i); i++);
}

bool BuffPool::ReFillPool(int buffID)
{
	switch (buffID)
	{
		RegBuff(BleedBuff)

	default:
		return false;
	}

	return true;
}

Buff* BuffPool::GetInstance(int buffID)
{
	Buff* result = nullptr;

	for (int i = 0; i < poolContent[buffID].size(); i++)
	{
		if (poolContent[buffID][i] == nullptr) continue;
		result = poolContent[buffID][i];
		poolContent[buffID][i] = nullptr;
		break;
	}

	if (result == nullptr)
	{
		ReFillPool(buffID);
		return GetInstance(buffID);
	}

	result->Initialize();

	return result;
}

void BuffPool::ReturnInstance(Buff* pInstance)
{
	pInstance->UnRegister();
	auto array = &poolContent[pInstance->buffID];

	for (int i = 0; i < array->size(); i++)
	{
		if ((*array)[i] != nullptr) continue;
		(*array)[i] = pInstance;
		return;
	}
}