#include "pch.h"
#include "CombatEncounter.h"
#include "TestEnemy.h"
#include "EncounterPool.h"
#include "CharacterPool.h"
#include "AdventureLog.h"

bool CombatEncounter::IsOngoing()
{
    return BaseEncounter::NPCs[0]->IsAlive();
}

void CombatEncounter::InternalInitialize(std::unique_ptr<Character>* pPlayer)
{
    AdventureLog::BeginNewSection();

    LocalizedString* log;
    AdventureLog::AddLog(log, "CombatEncounter");

    BaseEncounter::NPCs[0] = CharacterPool<TestEnemy>().GetInstance();
    pPlayer->get()->EngageInCombat(BaseEncounter::NPCs[0]);
    BaseEncounter::NPCs[0]->EngageInCombat(pPlayer->get());
}

void CombatEncounter::InternalTick(std::unique_ptr<Character>* pPlayer)
{
    
}

void CombatEncounter::InternalEnd(std::unique_ptr<Character>* pPlayer)
{
    pPlayer->get()->DisEngageInCombat();
}

void CombatEncounter::InternalReturnToPool()
{
    EncounterPool<CombatEncounter>().ReturnInstance(this);
}
