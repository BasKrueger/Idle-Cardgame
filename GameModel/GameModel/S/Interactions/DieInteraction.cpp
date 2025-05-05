#include "H/DieInteraction.h"
#include "H/Character.h"
#include "H/InteractionPool.h"

void DieInteraction::InternalPerform()
{
	if (pTarget == 0) return;

	pTarget->pOwner->Die(this);
}

void DieInteraction::ReturnToPool()
{
	InteractionPool<DieInteraction>().ReturnInstance(this);
}
