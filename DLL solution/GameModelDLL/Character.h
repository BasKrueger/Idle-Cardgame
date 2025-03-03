#pragma once
#include <array>
#include <iostream>
#include "InterActor.h"
#include "Json.hpp"
#include "LocalizedString.h"

class DamageInteraction;
class DieInteraction;
class BaseCard;

class Character : public InterActor
{
public:
	void Initialize();
	void Tick();
	void TryPlayNextCard(BaseCard* card);
	bool IsAlive();

	void EngageInCombat(Character* pNewEnemy);
	void DisEngageInCombat();
	void TakeDamage(DamageInteraction* interaction);
	void Die(DieInteraction* interaction);
	void ReturnCharacterToPool();

	json::JSON* GetState();
	LocalizedString* characterName = 0;

protected:
	enum PHASE{IDLE, COMBAT,DEAD};
	enum TRIGGERANIMATION {DAMAGED, PLAYED_CARD, NONE };
	int id = reinterpret_cast<uint32_t>(this);

	int dmg;
	int hp;
	int deckSize = 20;
	int currentCard;

	std::array<BaseCard*, 20> deck;
	Character* pEnemy;
	PHASE currentPhase;
	TRIGGERANIMATION nextAnimationTrigger;

	virtual void InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName);
	virtual void InternalReturnToPool();

	std::unique_ptr<json::JSON> state = 0;

private:
	int baseHP;
	int baseDamage;	
};