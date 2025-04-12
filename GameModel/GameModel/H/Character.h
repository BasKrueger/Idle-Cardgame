#pragma once
#include <array>
#include <iostream>
#include "H/InterActor.h"
#include "H/Json.hpp"
#include "H/LocalizedString.h"
#include "H/IDManager.h"
#include "H/Deck.h"

class DamageInteraction;
class HealInteraction;
class DieInteraction;
class BaseCard;

class Character : public InterActor
{
public:
	int characterID = -1;
	LocalizedString* characterName = nullptr;
	Deck* deck = nullptr;

	void Initialize();
	void Tick();
	bool IsAlive();

	void EngageInCombat(Character* pNewEnemy);
	void DisengageInCombat();
	void TakeDamage(DamageInteraction* interaction);
	void Heal(HealInteraction* interaction);
	void Die(DieInteraction* interaction);
	void ReturnCharacterToPool();

protected:
	int hp = 0;
	virtual void InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName, Deck* deck);

private:
	enum PHASE { IDLE, COMBAT, DEAD };
	enum TRIGGERANIMATION { DAMAGED, PLAYED_CARD, NONE };

	PHASE currentPhase = PHASE::IDLE;
	TRIGGERANIMATION nextAnimationTrigger = TRIGGERANIMATION::NONE;
	Character* pEnemy = nullptr;

	int id = IDManager::NewID();
	int dmg = 0;
	int baseDamage = 0;
	int baseHP = 0;
	
	void TryPlayNextCard(InterActor* pTarget);
	void PlayCard(BaseCard* pCard, InterActor* pTarget);

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static Character* LoadSave(json::JSON save);

protected:
	void SetSave(json::JSON save);

private:
	json::JSON* state = nullptr;
#pragma endregion
};