#include "H/DamageInteraction.h"
#include "H/InteractionPool.h"
#include "H/Character.h"

void DamageInteraction::Initialize(int damage)
{
	attackDamage = damage;
}

void DamageInteraction::InternalPerform()
{
	if (pTarget == 0) return;
	if (attackDamage <= 0) return;

	BaseInteraction::pTarget->pOwner->TakeDamage(this);
}

void DamageInteraction::ReturnToPool()
{
	InteractionPool<DamageInteraction>().ReturnInstance(this);
}
