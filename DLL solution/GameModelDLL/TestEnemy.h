#pragma once
#include "Character.h"

class TestEnemy : public Character
{
protected:
	void InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName) override;
	void InternalReturnToPool() override;
};

