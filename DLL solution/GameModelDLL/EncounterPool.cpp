#include "pch.h"
#include "EncounterPool.h"
#include "BaseEncounter.h"
#include "CombatEncounter.h"
#include "IdleEncounter.h"

#define RegEncounter(EncounterClassName) case EncounterClassName::encounterID: \
poolContent[EncounterClassName::encounterID].push_back(new EncounterClassName()); \
break; \

std::map<int, std::vector<BaseEncounter*>> EncounterPool::poolContent;

void EncounterPool::CreateInstance(int encounterID)
{
	switch (encounterID)
	{
		RegEncounter(IdleEncounter);
		RegEncounter(CombatEncounter);
	}
}

BaseEncounter* EncounterPool::GetInstance(std::unique_ptr<Player>* pPlayer, int encounterID)
{
	if (!poolContent.count(encounterID))
	{
		CreateInstance(encounterID);
	}

	if (poolContent[encounterID].size() == 0)
	{
		CreateInstance(encounterID);
	}

	BaseEncounter* result = poolContent[encounterID].back();
	poolContent[encounterID].pop_back();

	result->Initialize(pPlayer);
	return result;
}

void EncounterPool::ReturnInstance(BaseEncounter* pInstance)
{
	poolContent[pInstance->encounterID].push_back(pInstance);
}
