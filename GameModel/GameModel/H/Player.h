#pragma once
#include "Character.h"
#include "DamageInteraction.h"
#include <memory>
#include "Json.hpp"
#include <vector>

class Player : public Character
{
public:
	static const int characterID = 0;

	void Before(DamageInteraction* interaction) override;
	void AddToCollection(BaseCard* card);
	int SwitchCards(int collection, int deck);

	int xp = 1;
	int gold = 1;

private:
	std::vector<BaseCard*> collection;

protected:
	void InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName) override;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();

	void SetSave(json::JSON save);
private:
	json::JSON* state;

#pragma endregion
};

