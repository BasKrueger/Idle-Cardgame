#pragma once
#include "BaseCard.h"
#include <iostream>

class AttackCard : public BaseCard
{
public:
	static const int cardID = 0;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

