#pragma once
#include "BaseCard.h"
#include <iostream>

class FastAttackCard : public BaseCard
{
public:
	static const int cardID = 1;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int&baseHealing, int& baseCooldown, 
		std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

