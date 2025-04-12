#include "H/Character.h"
#include "H/BaseCard.h"
#include "H/DamageInteraction.h"
#include "H/DieInteraction.h"
#include "H/CardPool.h"
#include "H/InteractionManager.h"
#include "H/Game.h"
#include "H/Json.hpp"
#include "H/RewardStash.h"
#include "H/CharacterPool.h"
#include "H/HealInteraction.h"

void Character::Initialize()
{
	if (characterName == 0) characterName = new LocalizedString(LocalizedString::TABLE::CHARACTERS, "Missing Name");

	if (deck != nullptr) delete deck;
	deck = new Deck;

	InternalInitialize(characterID, baseHP, baseDamage, characterName, deck);

	hp = baseHP;
	dmg = baseDamage;
	
	InterActor::pOwner = this;
}

void Character::Tick()
{
	if (pEnemy == nullptr) return;

	if(!pEnemy->IsAlive())
	{
		DisengageInCombat();
		return;
	}

	deck->GetCurrentCard()->Tick();
	TryPlayNextCard(pEnemy);
}

void Character::TryPlayNextCard(InterActor* target)
{
	if (deck->GetCurrentCard()->IsCharged())
	{
		PlayCard(deck->GetCurrentCard(), target);
		deck->AdvanceToNextCard();
	}
}

void Character::PlayCard(BaseCard* pCard, InterActor* pTarget)
{
	nextAnimationTrigger = TRIGGERANIMATION::PLAYED_CARD;
	Game::CaptureGameState();

	auto interaction = InteractionPool<PlayInteraction>().GetInstance(this, pTarget);
	interaction->Initialize(pCard);
	InteractionManager::AddNext(interaction);

	pCard->Reset();
}

void Character::EngageInCombat(Character* pNewEnemy)
{
	pEnemy = pNewEnemy;
	currentPhase = PHASE::COMBAT;
	Game::CaptureGameState();
}

void Character::DisengageInCombat()
{
	pEnemy = nullptr;
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

void Character::Heal(HealInteraction* interaction)
{
	hp += interaction->healing;
	if (hp > baseHP) 
	{
		hp = baseHP;
	}

	nextAnimationTrigger = TRIGGERANIMATION::DAMAGED;
	Game::CaptureGameState();
}

void Character::ReturnCharacterToPool()
{ 
	deck->ReturnContentToPool();
	CharacterPool::ReturnInstance(this);
}

void Character::InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName, Deck* deck){}
void Character::Die(DieInteraction* interaction) { currentPhase = PHASE::DEAD; RewardStash::UnlockReward(0); }
bool Character::IsAlive() { return currentPhase != PHASE::DEAD; }

#pragma region State/Save/Load

json::JSON* Character::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["id"] = id;
	(*state)["baseHP"] = baseHP;
	(*state)["hp"] = hp;
	(*state)["dmg"] = dmg;
	(*state)["phase"] = (int)currentPhase;
	(*state)["animationTrigger"] = (int)nextAnimationTrigger;
	(*state)["autoDeck"] = *deck->GetState();

	nextAnimationTrigger = TRIGGERANIMATION::NONE;

	return state;
}

json::JSON Character::GetSave()
{
	json::JSON save;

	save["characterID"] = characterID;
	save["hp"] = hp;
	save["dmg"] = dmg;
	save["phase"] = (int)currentPhase;
	save["autoDeck"] = deck->GetSave();

	return save;
}

void Character::SetSave(json::JSON save)
{
	nextAnimationTrigger = TRIGGERANIMATION::NONE;
	hp = save["hp"].ToInt();
	dmg = save["dmg"].ToInt();
	currentPhase = static_cast<PHASE>(save["phase"].ToInt());

	deck->SetSave(save["autoDeck"], this);
	deck->SetCardsRegistered(true);
}

Character* Character::LoadSave(json::JSON save)
{
	Character* character = CharacterPool::GetInstance(save["characterID"].ToInt());
	character->SetSave(save);
	character->Register();
	return character;
}

#pragma endregion