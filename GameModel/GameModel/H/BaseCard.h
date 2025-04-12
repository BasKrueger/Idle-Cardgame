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
	int dmg = 0;
	int healing = 0;
	int cooldown = 0;
	std::array<int, 3> variables;

	void Initialize();
	void Reset();

	void Tick();
	bool IsCharged();
	void ReturnToPool();

	virtual void Play(PlayInteraction* pTarget);

protected:
	virtual void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
		std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName);

private:
	int baseDmg = 0;
	int baseCooldown = 0;
	int baseHealing = 0;
	
	std::array<int, 3> baseVariables;
	std::string iconName = "undefined";

	LocalizedString* cardDescription;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static BaseCard* LoadSave(Character* owner, json::JSON json);

private:
	json::JSON* state = nullptr;
#pragma endregion
};