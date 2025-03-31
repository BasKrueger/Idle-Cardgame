#include "pch.h"
#include "BaseEncounter.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "EncounterPool.h"
#include "Json.hpp"
#include "Player.h"

void BaseEncounter::Initialize(std::unique_ptr<Player>* pPlayer)
{
	InternalInitialize(pPlayer, hasStaticBackground, encounterID);
}

void BaseEncounter::End(std::unique_ptr<Player>* pPlayer)
{
	InternalEnd(pPlayer);
}

void BaseEncounter::ReturnToPool()
{
	for (int i = 0; i < std::size(variables); i++)
	{
		variables[i] = 0;
	}

	for (int i = 0; i < std::size(NPCs); i++)
	{
		if (NPCs[i] == nullptr) continue;

		NPCs[i]-> ReturnCharacterToPool();
		NPCs[i] = 0;
	}

	EncounterPool::ReturnInstance(this);
}

bool BaseEncounter::Tick(std::unique_ptr<Player>* pPlayer)
{
	pPlayer->get()->Tick();
	
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
void BaseEncounter::InternalInitialize(std::unique_ptr<Player>* pPlayer, bool& hasStaticBackground, int& encounterID){}
void BaseEncounter::InternalEnd(std::unique_ptr<Player>* pPlayer){}
void BaseEncounter::InternalTick(std::unique_ptr<Player>* pPlayer){}

#pragma region State/Save/Load

json::JSON* BaseEncounter::GetState()
{
	if (state == 0) state = std::make_unique<json::JSON>();

	(*state)["id"] = id;
	(*state)["isStaticEncounter"] = hasStaticBackground;

	(*state)["NPCs"] = json::Array();
	for (auto npc : NPCs)
	{
		if (npc == nullptr) continue;
		(*state)["NPCs"].append(*npc->GetState());
	}

	return state.get();
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

BaseEncounter* BaseEncounter::LoadSave(std::unique_ptr<Player>* pPlayer, json::JSON save)
{
	BaseEncounter* encounter = EncounterPool::GetInstance(pPlayer, save["encounterID"].ToInt());

	for (int i = 0; i < encounter->NPCs.size(); i++)
	{
		if (encounter->NPCs[i] == nullptr) continue;
		encounter->NPCs[i]->ReturnCharacterToPool();
		encounter->NPCs[i] = nullptr;
	}

	int iterator = -1;
	for (auto& npc : save["NPCs"].ArrayRange())
	{
		iterator++;
		encounter->NPCs[iterator] = Character::LoadSave(npc);
	}

	for (int i = 0; i < encounter->variables.size(); i++)
	{
		encounter->variables[i] = 0;
	}

	iterator = -1;
	for (auto& var : save["variables"].ArrayRange())
	{
		iterator++;
		encounter->variables[iterator] = var.ToInt();
	}

	return encounter;
}

#pragma endregion
