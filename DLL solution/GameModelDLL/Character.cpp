#include "pch.h"
#include "Character.h"
#include "BaseCard.h"
#include "DamageInteraction.h"
#include "DieInteraction.h"
#include "CardPool.h"
#include "InteractionManager.h"
#include "Game.h"
#include "Json.hpp"

void Character::Initialize()
{
	if (characterName == 0) characterName = new LocalizedString(LocalizedString::TABLE::CHARACTERS, "Missing Name");

	InternalInitialize(baseHP, baseDamage, characterName);

	hp = baseHP;
	dmg = baseDamage;

	deckSize = 0;
	for(int i = 0;i < deck.size() && deck[i] != 0;i++)
	{
		deckSize++;
	}

	currentPhase = PHASE::IDLE;
	nextAnimationTrigger = TRIGGERANIMATION::NONE;
	InterActor::pOwner = this;
}

void Character::Tick()
{
	if (pEnemy == 0)
	{
		return;
	}

	if(!pEnemy->IsAlive())
	{
		DisEngageInCombat();
		return;
	}

	deck[currentCard]->Tick();
	TryPlayNextCard(deck[currentCard]);
}

void Character::TryPlayNextCard(BaseCard* card)
{
	if (card->IsCharged())
	{
		nextAnimationTrigger = TRIGGERANIMATION::PLAYED_CARD;
		Game::CaptureGameState();

		card->TryPlay(pEnemy);

		currentCard += 1;
		if (currentCard >= deckSize) 
		{
			currentCard = 0;
		}
	}
}

void Character::EngageInCombat(Character* pNewEnemy)
{
	pEnemy = pNewEnemy;
	currentPhase = PHASE::COMBAT;
	Game::CaptureGameState();
}

void Character::DisEngageInCombat()
{
	pEnemy = 0;
	currentPhase = PHASE::IDLE;
	Game::CaptureGameState();
}

void Character::TakeDamage(DamageInteraction* interaction)
{
	hp -= interaction->attackDamage + interaction->pSource->pOwner->dmg;

	nextAnimationTrigger = TRIGGERANIMATION::DAMAGED;
	Game::CaptureGameState();

	if (hp <= 0) 
	{
		auto dieInteraction = InteractionPool<DieInteraction>().GetInstance(interaction->pSource, this);
		InteractionManager::ResolveNow(dieInteraction);
	}
}

void Character::ReturnCharacterToPool()
{ 
	for (auto card : deck)
	{
		if (card == 0) continue;
		card->ReturnToPool();
	}

	InternalReturnToPool();
}

void Character::InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName){}
void Character::InternalReturnToPool(){}

void Character::Die(DieInteraction* interaction) { currentPhase = PHASE::DEAD; }
bool Character::IsAlive() { return currentPhase != PHASE::DEAD; }

json::JSON* Character::GetState()
{
	if (state == 0) state = std::make_unique<json::JSON>();

	(*state)["id"] = id;
	(*state)["baseHP"] = baseHP;
	(*state)["hp"] = hp;
	(*state)["dmg"] = dmg;
	(*state)["phase"] = (int)currentPhase;
	(*state)["animationTrigger"] = (int)nextAnimationTrigger;
	
	(*state)["autoDeck"] = json::Array();
	auto index = currentCard;
	for (int i = 0; i < deckSize; i++)
	{
		if (index >= deckSize)
		{
			index = 0;
		}

		(*state)["autoDeck"].append(*deck[index]->GetState());
		index++;
	}

	nextAnimationTrigger = TRIGGERANIMATION::NONE;

	return state.get();
}