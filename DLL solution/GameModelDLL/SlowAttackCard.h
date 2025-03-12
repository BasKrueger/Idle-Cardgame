#pragma once
#include "BaseCard.h"
#include <iostream>

class SlowAttackCard : public BaseCard
{
public:
	static const int rewardID = 2;
	virtual void ReturnToPool() override;

protected:
	void InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

