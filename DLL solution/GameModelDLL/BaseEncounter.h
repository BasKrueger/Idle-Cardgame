#pragma once
#include "Character.h"
#include "Json.hpp"

class Player;
class Character;

class BaseEncounter
{
public:
	int encounterID;

	void Initialize(std::unique_ptr<Player>* pPlayer);
	bool Tick(std::unique_ptr<Player>* pPlayer);
	void End(std::unique_ptr<Player>* pPlayer);
	void ReturnToPool();
	
protected:
	std::array<Character*, 3> NPCs;
	std::array<int, 2> variables;

	virtual void InternalInitialize(std::unique_ptr<Player>* pPlayer, bool& staticBackground, int& encounterID);
	virtual void InternalTick(std::unique_ptr<Player>* pPlayer);
	virtual void InternalEnd(std::unique_ptr<Player>* pPlayer);

	virtual bool IsOngoing();

private:
	int id = reinterpret_cast<uint32_t>(this);
	bool hasStaticBackground = true;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	static BaseEncounter* LoadSave(std::unique_ptr<Player>* pPlayer, json::JSON save);

private:
	std::unique_ptr<json::JSON> state;
#pragma endregion
};