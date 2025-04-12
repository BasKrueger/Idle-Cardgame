#pragma once
#include "BaseCard.h"
#include <iostream>

class HealPotionCard : public BaseCard
{
public:
	static const int cardID = 3;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
		std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

