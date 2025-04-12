#pragma once
#include "Character.h"
#include "DamageInteraction.h"
#include <memory>
#include "Json.hpp"
#include <vector>
#include "Deck.h"

class Player : public Character
{
public:
	static const int characterID = 0;

	void Before(DamageInteraction* interaction) override;
	void After(DieInteraction* interaction) override;

	void AddToCollection(BaseCard* card);
	void SwitchCards(int ID1, int ID2);

	int xp = 1;
	int gold = 1;

private:
	Deck collection;

protected:
	void InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName, Deck* deck) override;

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();

	void SetSave(json::JSON save);
private:
	json::JSON* state;

#pragma endregion
};

