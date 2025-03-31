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
	LocalizedString* characterName = nullptr;
	int characterID;

	void Initialize();
	void Tick();
	bool IsAlive();

	void EngageInCombat(Character* pNewEnemy);
	void DisengageInCombat();
	void TakeDamage(DamageInteraction* interaction);
	void Die(DieInteraction* interaction);
	void ReturnCharacterToPool();


protected:
	std::array<BaseCard*, 20> deck;
	int hp;
	
	virtual void InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName);

private:
	enum PHASE { IDLE, COMBAT, DEAD };
	enum TRIGGERANIMATION { DAMAGED, PLAYED_CARD, NONE };

	PHASE currentPhase = PHASE::IDLE;
	TRIGGERANIMATION nextAnimationTrigger = TRIGGERANIMATION::NONE;
	Character* pEnemy = nullptr;

	int id = reinterpret_cast<uint32_t>(this);
	int dmg;
	int baseDamage;
	int baseHP;
	int currentCardIndex;
	
	void TryPlayNextCard(BaseCard* card);

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static Character* LoadSave(json::JSON save);

protected:
	void SetSave(json::JSON save);

private:
	std::unique_ptr<json::JSON> state = nullptr;
#pragma endregion

};