#pragma once
#include "Character.h"

class TestEnemy : public Character
{
public:
	static const int characterID = 1;

protected:
	void InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName, Deck* deck) override;
};

