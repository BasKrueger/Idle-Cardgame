#include "H/HealInteraction.h"
#include "H/InteractionPool.h"
#include "H/Character.h"
#include "H/AdventureLog.h"

void HealInteraction::Initialize(int heal)
{
	healing = heal;
}

void HealInteraction::InternalPerform()
{
	if (pTarget == 0) return;

	LocalizedString* log;
	if (AdventureLog::AddLog(log, "Heal"))
	{
		log->BindFormatVariable(pSource->pOwner->characterName);
		log->BindFormatVariable(std::to_string(healing));
		log->BindFormatVariable(pSource->pOwner->characterName);
	}

	BaseInteraction::pSource->pOwner->Heal(this);
}

void HealInteraction::ReturnToPool()
{
	InteractionPool<HealInteraction>().ReturnInstance(this);
}
