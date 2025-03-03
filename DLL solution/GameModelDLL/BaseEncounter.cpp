#include "pch.h"
#include "BaseEncounter.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "Json.hpp"


void BaseEncounter::BaseInitialize(std::unique_ptr<Character>* pPlayer)
{
	InternalInitialize(pPlayer);
}

void BaseEncounter::End(std::unique_ptr<Character>* pPlayer)
{
	InternalEnd(pPlayer);
}

void BaseEncounter::ReturnEncounterToPool()
{
	for (int i = 0; i < std::size(variables); i++)
	{
		variables[i] = 0;
	}

	for (int i = 0; i < std::size(NPCs); i++)
	{
		if (NPCs[i] == 0) continue;

		NPCs[i]-> ReturnCharacterToPool();
		NPCs[i] = 0;
	}

	InternalReturnToPool();
}

json::JSON* BaseEncounter::GetState()
{
	if (state == 0) state = std::make_unique<json::JSON>();

	(*state)["id"] = id;
	(*state)["isStaticEncounter"] = IsStaticEncounter();

	(*state)["NPCs"] = json::Array();
	for(auto npc : NPCs)
	{
		if (npc == 0) continue;
		(*state)["NPCs"].append(*npc->GetState());
	}

	return state.get();
}

bool BaseEncounter::Tick(std::unique_ptr<Character>* pPlayer)
{
	pPlayer->get()->Tick();
	
	for (auto& npc : NPCs)
	{
		if (npc != 0) 
		{
			npc->Tick();
		}
	}

	InternalTick(pPlayer);
	
	return IsOngoing();
}

bool BaseEncounter::IsOngoing()
{
	return false;
}

void BaseEncounter::InternalReturnToPool()
{
	InteractionPool<BaseEncounter>().ReturnInstance(this);
}

bool BaseEncounter::IsStaticEncounter()
{
	return true;
}

void BaseEncounter::InternalInitialize(std::unique_ptr<Character>* pPlayer)
{
}

void BaseEncounter::InternalEnd(std::unique_ptr<Character>* pPlayer)
{
}

void BaseEncounter::InternalTick(std::unique_ptr<Character>* pPlayer)
{
}
