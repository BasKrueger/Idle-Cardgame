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
	void Initialize();
	void Reset();

	void Tick();
	bool IsCharged();
	bool TryPlay(InterActor* pTarget);
	virtual void Play(PlayInteraction* pTarget);

	virtual void ReturnToPool();

	json::JSON* GetState();
	LocalizedString* cardName;

protected:
	int cooldown;
	int dmg;
	int id = reinterpret_cast<uint32_t>(this);

	virtual void InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription);

private:
	int baseDmg;
	int baseCooldown;

	std::array<int, 3> variables;

	LocalizedString* cardDescription;
	std::unique_ptr<json::JSON> state = 0;
};