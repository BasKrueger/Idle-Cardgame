#pragma once
#include <array>
#include <iostream>
#include "H/InterActor.h"
#include "H/Json.hpp"
#include "H/LocalizedString.h"
#include "H/IDManager.h"

class Character;
class PlayInteraction;

class BaseCard : public InterActor
{
public:
	LocalizedString* cardName;
	int id = IDManager::NewID();
	int cardID = -1;

	void Initialize();
	void Reset();

	void Tick();
	bool IsCharged();
	bool TryPlay(InterActor* pTarget);
	void ReturnToPool();

	virtual void Play(PlayInteraction* pTarget);

protected:
	int cooldown;
	int dmg;

	virtual void InternalInitialize(int& cID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription);

private:
	int baseDmg;
	int baseCooldown;

	LocalizedString* cardDescription;
	std::array<int, 3> variables;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static BaseCard* LoadSave(Character* owner, json::JSON json);

private:
	json::JSON* state = nullptr;
#pragma endregion
};