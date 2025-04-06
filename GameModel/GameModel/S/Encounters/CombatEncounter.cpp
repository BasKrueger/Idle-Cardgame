#include "H/CombatEncounter.h"
#include "H/TestEnemy.h"
#include "H/EncounterPool.h"
#include "H/CharacterPool.h"
#include "H/AdventureLog.h"
#include "H/Player.h"

const int CombatEncounter::encounterID;

bool CombatEncounter::IsOngoing()
{
    return BaseEncounter::NPCs[0]->IsAlive();
}

void CombatEncounter::InternalInitialize(Player* pPlayer, bool& hasStaticBackground, int& eID)
{
    eID = encounterID;
    hasStaticBackground = true;

    AdventureLog::BeginNewSection();
    LocalizedString* log;
    AdventureLog::AddLog(log, "CombatEncounter");

    BaseEncounter::NPCs[0] = CharacterPool::GetInstance(TestEnemy::characterID);
    pPlayer->EngageInCombat(BaseEncounter::NPCs[0]);
    BaseEncounter::NPCs[0]->EngageInCombat(pPlayer);
}

void CombatEncounter::InternalTick(Player* pPlayer)
{
    
}

void CombatEncounter::InternalEnd(Player* pPlayer)
{
    pPlayer->DisengageInCombat();
}