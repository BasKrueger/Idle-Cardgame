#include "H/DieInteraction.h"
#include "H/Character.h"
#include "H/InteractionPool.h"
#include "H/AdventureLog.h"

void DieInteraction::InternalPerform()
{
	if (pTarget == 0) return;

	LocalizedString* log;
	if (AdventureLog::AddLog(log, "Death"))
	{
		log->BindFormatVariable(pTarget->pOwner->characterName);
	}

	pTarget->pOwner->Die(this);
}

void DieInteraction::ReturnToPool()
{
	InteractionPool<DieInteraction>().ReturnInstance(this);
}
