#pragma once
#include "H/InterActor.h"
#include "H/Json.hpp"
#include "H/IDManager.h"
#include <array>

class BaseCard;
class Character;

class Buff : public InterActor
{
public:
	BaseCard* pCard;

	int id = IDManager::NewID();
	int buffID = -1;

	void Initialize();
	void Tick();

	virtual void OnApply();
	virtual void OnRemove();

protected:
	int duration = -1;
	std::array<int, 3> variables;

	virtual void OnTick();
	virtual void InternalInitialize(int& id, int& duration, std::array<int, 3>& variables, std::string& iconName);

private:
	std::array<int, 3> baseVariables;
	std::string iconName = "undefined";

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	void SetSave(json::JSON save);
	static Buff* LoadSave(BaseCard* owner, json::JSON json);

private:
	json::JSON* state = nullptr;
#pragma endregion
};

