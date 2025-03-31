#pragma once
#include <array>
#include <iostream>
#include "InterActor.h"
#include "Json.hpp"
#include "LocalizedString.h"

class Character;
class PlayInteraction;

class BaseCard : public InterActor
{
public:
	LocalizedString* cardName;
	int id = reinterpret_cast<uint32_t>(this);
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
	std::unique_ptr<json::JSON> state = nullptr;
#pragma endregion
};