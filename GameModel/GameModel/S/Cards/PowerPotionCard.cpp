#include "H/PowerPotionCard.h"
#include "H/PlayInteraction.h"
#include "H/Character.h"

const int PowerPotionCard::cardID;

void PowerPotionCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardText, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 4;
	baseVariables[0] = 1;

	iconName = "Set1/PowerPotion";
	cardText->SetKey("PowerPotion");
	cardText->BindFormatVariable(&baseVariables[0]);
}

void PowerPotionCard::Play(PlayInteraction* pPlay)
{
	pPlay->pSource->pOwner->deck->GetNextCard()->dmg += variables[0];
}
