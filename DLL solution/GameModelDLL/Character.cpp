#include "pch.h"
#include "Character.h"
#include "BaseCard.h"
#include "DamageInteraction.h"
#include "DieInteraction.h"
#include "CardPool.h"
#include "InteractionManager.h"
#include "Game.h"
#include "Json.hpp"
#include "RewardStash.h"
#include "CharacterPool.h"

void Character::Initialize()
{
	if (characterName == 0) characterName = new LocalizedString(LocalizedString::TABLE::CHARACTERS, "Missing Name");

	InternalInitialize(characterID, baseHP, baseDamage, characterName);

	hp = baseHP;
	dmg = baseDamage;
	
	InterActor::pOwner = this;
}

void Character::Tick()
{
	if (pEnemy == nullptr) return;
	if (pEnemy->hp <= 0) return;

	if(!pEnemy->IsAlive())
	{
		DisengageInCombat();
		return;
	}

	deck[currentCardIndex]->Tick();
	TryPlayNextCard(deck[currentCardIndex]);

}

void Character::TryPlayNextCard(BaseCard* card)
{
	if (card->IsCharged())
	{
		nextAnimationTrigger = TRIGGERANIMATION::PLAYED_CARD;
		Game::CaptureGameState();

		card->TryPlay(pEnemy);

		currentCardIndex += 1;
		if (currentCardIndex > deck.size() - 1 || deck[currentCardIndex] == nullptr)
		{
			currentCardIndex = 0;
		}
	}
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

void Character::ReturnCharacterToPool()
{ 
	for (auto card : deck)
	{
		if (card == nullptr) continue;
		card->ReturnToPool();
	}

	CharacterPool::ReturnInstance(this);
}

void Character::InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName){}
void Character::Die(DieInteraction* interaction) { currentPhase = PHASE::DEAD; RewardStash::UnlockReward(0); }
bool Character::IsAlive() { return currentPhase != PHASE::DEAD; }

#pragma region State/Save/Load

json::JSON* Character::GetState()
{
	if (state == 0) state = std::make_unique<json::JSON>();

	(*state)["id"] = id;
	(*state)["baseHP"] = baseHP;
	(*state)["hp"] = hp;
	(*state)["dmg"] = dmg;
	(*state)["phase"] = (int)currentPhase;
	(*state)["animationTrigger"] = (int)nextAnimationTrigger;
	(*state)["currentCard"] = currentCardIndex;

	(*state)["autoDeck"] = json::Array();
	for (auto& card : deck)
	{
		if (card == nullptr) continue;
		(*state)["autoDeck"].append(*card->GetState());
	}

	nextAnimationTrigger = TRIGGERANIMATION::NONE;

	return state.get();
}

json::JSON Character::GetSave()
{
	json::JSON save;

	save["characterID"] = characterID;
	save["hp"] = hp;
	save["dmg"] = dmg;
	save["phase"] = (int)currentPhase;
	save["currentCard"] = currentCardIndex;

	save["autoDeck"] = json::Array();
	for (auto& card : deck)
	{
		if (card == nullptr) continue;
		save["autoDeck"].append(card->GetSave());
	}

	return save;
}

void Character::SetSave(json::JSON save)
{
	nextAnimationTrigger = TRIGGERANIMATION::NONE;
	hp = save["hp"].ToInt();
	dmg = save["dmg"].ToInt();
	currentCardIndex = save["currentCard"].ToInt();
	currentPhase = static_cast<PHASE>(save["phase"].ToInt());

	for (int i = 0; i < deck.size(); i++)
	{
		if (deck[i] == nullptr) continue;
		deck[i]->ReturnToPool();
		deck[i] = nullptr;
	}

	int iterator = -1;
	for (auto& card : (save)["autoDeck"].ArrayRange())
	{
		iterator++;
		deck[iterator] = BaseCard::LoadSave(this, card);
	}
}

Character* Character::LoadSave(json::JSON save)
{
	Character* character = CharacterPool::GetInstance(save["characterID"].ToInt());
	character->SetSave(save);
	return character;
}

#pragma endregion