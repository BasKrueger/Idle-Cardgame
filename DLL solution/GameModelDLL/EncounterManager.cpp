#include "pch.h"
#include "EncounterManager.h"
#include "EncounterPool.h"
#include "Json.hpp"
#include "Player.h"

EncounterManager::~EncounterManager()
{
    pCurrentEncounter->ReturnToPool();
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

void EncounterManager::BeginNextEncounter()
{
    if (pCurrentEncounter != 0) 
    {
        pCurrentEncounter->End(pPlayer);
        pCurrentEncounter->ReturnToPool();
    }

    pCurrentEncounter = GenerateNextEncounter();
}

BaseEncounter* EncounterManager::GenerateNextEncounter()
{
    encounterIndex += 1;

    if (encounterIndex % 2 == 0)
    {
        return EncounterPool::GetInstance(pPlayer, 0);
    }

    return EncounterPool::GetInstance(pPlayer, 1);
}


#pragma region State/Save/Load
json::JSON* EncounterManager::GetState()
{
    return pCurrentEncounter->GetState();
}

json::JSON EncounterManager::GetSave()
{
    json::JSON save;
    save["encounterIndex"] = encounterIndex;
    save["encounter"] = pCurrentEncounter->GetSave();

    return save;
}

void EncounterManager::SetSave(std::unique_ptr<Player>* pPlayer, json::JSON save)
{
    pCurrentEncounter->End(pPlayer);
    pCurrentEncounter->ReturnToPool();

    encounterIndex = save["encounterIndex"].ToInt();
    pCurrentEncounter = BaseEncounter::LoadSave(pPlayer, save["encounter"]);
}

#pragma endregion