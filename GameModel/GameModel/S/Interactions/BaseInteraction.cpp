#include "h/BaseInteraction.h"
#include "h/InteractionPool.h"

void BaseInteraction::Perform()
{
	InternalPerform();
	ReturnToPool();
}

void BaseInteraction::InternalPerform()
{
}

void BaseInteraction::ReturnToPool()
{
	InteractionPool<BaseInteraction>().ReturnInstance(this);
}

void BaseInteraction::BaseInitialization(InterActor* interactionSource, InterActor* interactionTarget)
{
	pSource = interactionSource;
	pTarget = interactionTarget;
}
