#include "pch.h"
#include "EncounterManager.h"
#include "CombatEncounter.h"
#include "IdleEncounter.h"
#include "EncounterPool.h"
#include "Json.hpp"
#include "Player.h"

EncounterManager::~EncounterManager()
{
    pCurrentEncounter->ReturnEncounterToPool();
}

EncounterManager::EncounterManager(std::unique_ptr<Player>* pPlayer)
{
    this->pPlayer = pPlayer;
    BeginNextEncounter();
}

void EncounterManager::Tick()
{
    if (!pPlayer->get()->IsAlive()) 
    {
        return;
    }

    if (!pCurrentEncounter->Tick(pPlayer)) 
    {
        BeginNextEncounter();
    }
}

json::JSON* EncounterManager::GetState()
{
    return pCurrentEncounter->GetState();
}

void EncounterManager::BeginNextEncounter()
{
    if (pCurrentEncounter != 0) 
    {
        pCurrentEncounter->End(pPlayer);
        pCurrentEncounter->ReturnEncounterToPool();
    }

    pCurrentEncounter = GenerateNextEncounter();
}

BaseEncounter* EncounterManager::GenerateNextEncounter()
{
    encounterIndex += 1;

    if (encounterIndex % 2 == 0)
    {
        return EncounterPool<IdleEncounter>().GetInstance(pPlayer);
    }

    return EncounterPool<CombatEncounter>().GetInstance(pPlayer);
}
