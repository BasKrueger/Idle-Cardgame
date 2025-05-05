#pragma once
#include "H/BaseCard.h"

class TieUpCard : public BaseCard
{
public:
	static const int cardID = 10;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
		std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pPlay) override;
};