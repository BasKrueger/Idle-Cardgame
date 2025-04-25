#include "H/RemoveBuffInteraction.h"
#include "H/InteractionPool.h"
#include "H/BaseCard.h"

void RemoveBuffInteraction::Initialize(Buff* buff, BaseCard* target)
{
	this->buff = buff;
	this->target = target;
}

void RemoveBuffInteraction::InternalPerform()
{
	target->RemoveBuff(this);
}

void RemoveBuffInteraction::ReturnToPool()
{
	InteractionPool<RemoveBuffInteraction>().ReturnInstance(this);
}
