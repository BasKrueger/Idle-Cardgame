#pragma once
#include "BaseCard.h"
#include <iostream>

class FastAttackCard : public BaseCard
{
public:
	static const int rewardID = 1;
	virtual void ReturnToPool() override;

protected:
	void InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

