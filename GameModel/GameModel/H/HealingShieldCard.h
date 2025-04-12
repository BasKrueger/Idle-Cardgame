#pragma once
#include "H/BaseCard.h"

class HealingShieldCard : public BaseCard
{
public:
	static const int cardID = 9;

protected:
	void InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
		std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName) override;

private:
	virtual void Play(PlayInteraction* pPlay) override;
	virtual void Before(DamageInteraction* interaction) override;
};