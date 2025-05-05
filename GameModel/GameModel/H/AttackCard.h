#pragma once
#include "BaseCard.h"
#include <iostream>

class AttackCard : public BaseCard
{
public:
	static const int cardID = 0;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int&baseHealing, int& baseCooldown, 
		std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pTarget) override;
};

