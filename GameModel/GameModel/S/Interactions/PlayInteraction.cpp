#include "H/PlayInteraction.h"
#include "H/BaseCard.h"
#include "H/InteractionPool.h"
#include "H/Character.h"
#include "H/AdventureLog.h"

void PlayInteraction::Initialize(BaseCard* card)
{
	cardToPlay = card;
}

void PlayInteraction::InternalPerform()
{
	if (cardToPlay == 0) return;

	LocalizedString* log;
	if (AdventureLog::AddLog(log, "CardPlayed"))
	{
		log->BindFormatVariable(pSource->pOwner->characterName);
		log->BindFormatVariable(cardToPlay->cardName);
	}

	cardToPlay->Play(this);
}

void PlayInteraction::ReturnToPool()
{
	cardToPlay = 0;
	InteractionPool<PlayInteraction>().ReturnInstance(this);
}
