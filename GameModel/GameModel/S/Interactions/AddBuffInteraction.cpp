#include "H/AddBuffInteraction.h"
#include "H/InteractionPool.h"
#include "H/BaseCard.h"
#include "H/Buff.h"
#include "H/Character.h"

void AddBuffInteraction::Initialize(Buff* buff, BaseCard* target)
{
	this->buff = buff;
	this->target = target;
}

void AddBuffInteraction::Initialize(Buff* buff, Character* target)
{
	Initialize(buff, target->deck->GetCurrentCard());
}

void AddBuffInteraction::InternalPerform()
{
	target->AttachBuff(this);
}

void AddBuffInteraction::ReturnToPool()
{
	InteractionPool<AddBuffInteraction>().ReturnInstance(this);
}
