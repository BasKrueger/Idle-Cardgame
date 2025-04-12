#include "H/BaseEncounter.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"
#include "H/EncounterPool.h"
#include "H/Json.hpp"
#include "H/Player.h"

void BaseEncounter::Initialize(Player* pPlayer)
{
	InternalInitialize(pPlayer, hasStaticBackground, encounterID);
}

void BaseEncounter::End(Player* pPlayer)
{
	InternalEnd(pPlayer);
}

void BaseEncounter::ReturnToPool()
{
	for (int i = 0; i < variables.size(); i++)
	{
		variables[i] = 0;
	}

	for (int i = 0; i < NPCs.size(); i++)
	{
		if (NPCs[i] == nullptr) continue;

		NPCs[i]-> ReturnCharacterToPool();
		NPCs[i] = nullptr;
	}

	EncounterPool::ReturnInstance(this);
}

bool BaseEncounter::Tick(Player* pPlayer)
{
	pPlayer->Tick();
	
	for (auto& npc : NPCs)
	{
		if (npc != nullptr) 
		{
			npc->Tick();
		}
	}

	InternalTick(pPlayer);
	
	return IsOngoing();
}

bool BaseEncounter::IsOngoing(){ return false; }

void BaseEncounter::SetNPC(int slot, Character* pCharacter)
{
	NPCs[slot] = pCharacter;
	pCharacter->Register();
}

Character* BaseEncounter::GetNPC(int slot)
{
	return NPCs[slot];
}

void BaseEncounter::InternalInitialize(Player* pPlayer, bool& hasStaticBackground, int& encounterID){}
void BaseEncounter::InternalEnd(Player* pPlayer){}
void BaseEncounter::InternalTick(Player* pPlayer){}

#pragma region State/Save/Load

json::JSON* BaseEncounter::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["id"] = id;
	(*state)["isStaticEncounter"] = hasStaticBackground;

	(*state)["NPCs"] = json::Array();
	for (auto npc : NPCs)
	{
		if (npc == nullptr) continue;
		(*state)["NPCs"].append(*npc->GetState());
	}

	return state;
}

json::JSON BaseEncounter::GetSave()
{
	json::JSON save;

	(save)["encounterID"] = encounterID;

	(save)["NPCs"] = json::Array();
	for (auto& npc : NPCs)
	{
		if (npc == nullptr) continue;
		(save)["NPCs"].append(npc->GetSave());
	}

	(save)["variables"] = json::Array();
	for (auto& var : variables)
	{
		(save)["variables"].append(var);
	}

	return save;
}

BaseEncounter* BaseEncounter::LoadSave(Player* pPlayer, json::JSON save)
{
	BaseEncounter* encounter = EncounterPool::GetInstance(pPlayer, save["encounterID"].ToInt());

	for (int i = 0; i < encounter->NPCs.size(); i++)
	{
		if (encounter->NPCs[i] == nullptr) continue;
		encounter->NPCs[i]->ReturnCharacterToPool();
		encounter->NPCs[i] = nullptr;
	}

	for (auto i = 0; i < save["NPCs"].size(); i++)
	{
		encounter->SetNPC(i, Character::LoadSave(save["NPCs"][i]));
	}

	for (int i = 0; i < encounter->variables.size(); i++)
	{
		encounter->variables[i] = 0;
	}

	for (auto i = 0 ; i < save["variables"].size();i++)
	{
		encounter->variables[i] = save["variables"][i].ToInt();
	}

	return encounter;
}

#pragma endregion
