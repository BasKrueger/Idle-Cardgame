#pragma once
#include "BaseCard.h"
#include <iostream>

class SlowAttackCard : public BaseCard
{
public:
	static const int cardID = 2;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

