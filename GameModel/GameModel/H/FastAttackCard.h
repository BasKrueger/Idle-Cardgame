#pragma once
#include "BaseCard.h"
#include <iostream>

class FastAttackCard : public BaseCard
{
public:
	static const int cardID = 1;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

