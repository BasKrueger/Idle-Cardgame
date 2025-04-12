#include "H/DamageInteraction.h"
#include "H/InteractionPool.h"
#include "H/Character.h"
#include "H/AdventureLog.h"

void DamageInteraction::Initialize(int damage)
{
	attackDamage = damage;
}

void DamageInteraction::InternalPerform()
{
	if (pTarget == 0) return;
	if (attackDamage <= 0) return;

	LocalizedString* log;
	if (AdventureLog::AddLog(log, "DamageTaken"))
	{
		log->BindFormatVariable(pTarget->pOwner->characterName);
		log->BindFormatVariable(std::to_string(attackDamage));
		log->BindFormatVariable(pSource->pOwner->characterName);
	}

	BaseInteraction::pTarget->pOwner->TakeDamage(this);
}

void DamageInteraction::ReturnToPool()
{
	InteractionPool<DamageInteraction>().ReturnInstance(this);
}
