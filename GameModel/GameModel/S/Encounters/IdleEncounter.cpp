#include "H/IdleEncounter.h"
#include "H/EncounterPool.h"
#include "H/Player.h"

const int IdleEncounter::encounterID;

bool IdleEncounter::IsOngoing()
{
	return BaseEncounter::variables[0] > 0;
}

void IdleEncounter::InternalInitialize(Player* pPlayer, bool& hasStaticBackground, int& eID)
{
	eID = encounterID;
	hasStaticBackground = false;

	BaseEncounter::variables[0] = 20;
}

void IdleEncounter::InternalEnd(Player* pPlayer)
{
	
}

void IdleEncounter::InternalTick(Player* pPlayer)
{
	BaseEncounter::variables[0] -= 1;
}
