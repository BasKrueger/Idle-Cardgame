#include "H/PlayInteraction.h"
#include "H/BaseCard.h"
#include "H/InteractionPool.h"
#include "H/Character.h"

void PlayInteraction::Initialize(BaseCard* card)
{
	toPlay = card;
}

void PlayInteraction::InternalPerform()
{
	if (toPlay == nullptr) return;

	toPlay->Play(this);
}

void PlayInteraction::ReturnToPool()
{
	toPlay = 0;
	InteractionPool<PlayInteraction>().ReturnInstance(this);
}
