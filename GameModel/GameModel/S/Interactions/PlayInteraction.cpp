#include "H/PlayInteraction.h"
#include "H/BaseCard.h"
#include "H/InteractionPool.h"
#include "H/Character.h"
#include "H/AdventureLog.h"

void PlayInteraction::Initialize(BaseCard* card)
{
	toPlay = card;
}

void PlayInteraction::InternalPerform()
{
	if (toPlay == nullptr) return;

	LocalizedString* log;
	if (AdventureLog::AddLog(log, "CardPlayed"))
	{
		log->BindFormatVariable(pSource->pOwner->characterName);
		log->BindFormatVariable(toPlay->cardName);
	}

	toPlay->Play(this);
}

void PlayInteraction::ReturnToPool()
{
	toPlay = 0;
	InteractionPool<PlayInteraction>().ReturnInstance(this);
}
