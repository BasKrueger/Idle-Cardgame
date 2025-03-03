#pragma once
#include "Character.h"
#include "Json.hpp"

class Character;

class BaseEncounter
{
public:
	bool Tick(std::unique_ptr<Character>* pPlayer);
	void BaseInitialize(std::unique_ptr<Character>* pPlayer);
	void End(std::unique_ptr<Character>* pPlayer);
	void ReturnEncounterToPool();

	json::JSON* GetState();
	
private:
	int id = reinterpret_cast<uint32_t>(this);
	std::unique_ptr<json::JSON> state;

	virtual void InternalInitialize(std::unique_ptr<Character>* pPlayer);
	virtual void InternalEnd(std::unique_ptr<Character>* pPlayer);
	virtual bool IsStaticEncounter();

	virtual void InternalTick(std::unique_ptr<Character>* pPlayer);
	virtual bool IsOngoing();
	virtual void InternalReturnToPool();

protected:
	std::array<Character*, 3> NPCs;
	std::array<int, 2> variables;
};