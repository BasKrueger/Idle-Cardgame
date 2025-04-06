#pragma once
#include "H/Character.h"
#include "H/Json.hpp"
#include "IDManager.h"
#include <array>
#include <vector>

class Player;
class Character;

class BaseEncounter
{
public:
	int encounterID;

	void Initialize(Player* pPlayer);
	bool Tick(Player* pPlayer);
	void End(Player* pPlayer);
	void ReturnToPool();
	
protected:
	std::array<Character*, 3> NPCs;
	std::array<int, 2> variables;

	virtual void InternalInitialize(Player* pPlayer, bool& staticBackground, int& encounterID);
	virtual void InternalTick(Player* pPlayer);
	virtual void InternalEnd(Player* pPlayer);

	virtual bool IsOngoing();

private:
	int id = IDManager::NewID();
	bool hasStaticBackground = true;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static BaseEncounter* LoadSave(Player* pPlayer, json::JSON save);

private:
	json::JSON* state;
#pragma endregion
};