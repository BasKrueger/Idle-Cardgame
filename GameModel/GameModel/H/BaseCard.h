#pragma once
#include <array>
#include <iostream>
#include "H/InterActor.h"
#include "H/Json.hpp"
#include "H/LocalizedString.h"
#include "H/IDManager.h"

class Character;
class Buff;
class PlayInteraction; 
class AddBuffInteraction;
class RemoveBuffInteraction;

class BaseCard : public InterActor
{
public:
	int id = IDManager::NewID();
	int cardID = -1;
	int dmg = 0;
	int healing = 0;
	int cooldown = 0;
	int baseCooldown = 0;
	std::array<int, 3> variables;

	void Initialize();
	void ResetCooldown();
	void FullReset();

	void Tick();
	void BuffTick();
	bool IsCharged();
	void ReturnToPool();

	void AttachBuff(AddBuffInteraction* pInteraction);
	void RemoveBuff(RemoveBuffInteraction* pInteraction);

	virtual void Play(PlayInteraction* pTarget);

protected:
	virtual void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
		std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName);

private:
	int baseDmg = 0;
	int baseHealing = 0;
	
	std::vector<Buff*> buffs;
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