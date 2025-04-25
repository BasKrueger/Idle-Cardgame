#include "H/TieUpCard.h"
#include "H/InteractionManager.h"

const int TieUpCard::cardID;

void TieUpCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 4;
	baseVariables[0] = 3;

	iconName = "Set2/Icon34";
	cardName->SetKey("TieUp_Name");
	cardDescription->SetKey("TieUp_Text");
	cardDescription->BindFormatVariable(&variables[0]);
}

void TieUpCard::Play(PlayInteraction* pPlay)
{
	pPlay->pTarget->pOwner->deck->GetNextCard()->cooldown += variables[0];
	pPlay->pTarget->pOwner->deck->GetNextCard()->baseCooldown += variables[0];
}
