#include "pch.h"
#include "IdleEncounter.h"
#include "EncounterPool.h"
#include "AdventureLog.h"
#include "Player.h"

bool IdleEncounter::IsOngoing()
{
	return BaseEncounter::variables[0] > 0;
}

void IdleEncounter::InternalInitialize(std::unique_ptr<Player>* pPlayer, bool& hasStaticBackground, int& eID)
{
	eID = encounterID;
	hasStaticBackground = false;

	AdventureLog::BeginNewSection();
	LocalizedString* log;
	if (AdventureLog::AddLog(log, "IdleEncounter"))
	{
		log->BindFormatVariable(pPlayer->get()->characterName);
	}

	BaseEncounter::variables[0] = 60;
}

void IdleEncounter::InternalEnd(std::unique_ptr<Player>* pPlayer)
{
	
}

void IdleEncounter::InternalTick(std::unique_ptr<Player>* pPlayer)
{
	BaseEncounter::variables[0] -= 1;
}
