#pragma once
#include "H/BaseCard.h"

class WoundingStrikeCard : public BaseCard
{
public:
	static const int cardID = 18;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
		std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pPlay) override;
	virtual void After(PlayInteraction* interaction) override;
};