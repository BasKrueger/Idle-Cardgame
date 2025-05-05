#include "H/HealInteraction.h"
#include "H/InteractionPool.h"
#include "H/Character.h"

void HealInteraction::Initialize(int heal)
{
	healing = heal;
}

void HealInteraction::InternalPerform()
{
	if (pTarget == 0) return;

	BaseInteraction::pSource->pOwner->Heal(this);
}

void HealInteraction::ReturnToPool()
{
	InteractionPool<HealInteraction>().ReturnInstance(this);
}
