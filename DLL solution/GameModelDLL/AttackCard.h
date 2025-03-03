#pragma once
#include "BaseCard.h"
#include <iostream>

class AttackCard : public BaseCard
{
public:
	virtual void ReturnToPool() override;

protected:
	void InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

