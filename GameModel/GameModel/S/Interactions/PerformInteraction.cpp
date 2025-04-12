#include "H/PerformInteraction.h"
#include "H/InteractionManager.h"

void PerformInteraction::Initialize(BaseInteraction* interaction)
{
	pInteraction = interaction;
}

void PerformInteraction::ReturnToPool()
{
	InteractionPool<PerformInteraction>().ReturnInstance(this);
}

void PerformInteraction::InternalPerform()
{
	if (pInteraction == 0) return;
	pInteraction->Perform();
}
