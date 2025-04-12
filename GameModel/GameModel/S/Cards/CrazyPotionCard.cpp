#include "H/CrazyPotionCard.h"
#include "H/PlayInteraction.h"
#include "H/Character.h";

const int CrazyPotionCard::cardID;

void CrazyPotionCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 1;

	iconName = "Set1/CrazyPotion";
	cardName->SetKey("CrazyPotion_Name");
	cardDescription->SetKey("CrazyPotion_Text");
	cardDescription->BindFormatVariable(&variables[0]);
}

void CrazyPotionCard::Play(PlayInteraction* pPlay)
{
	auto nextCard = pPlay->pSource->pOwner->deck->GetNextCard();

	nextCard->dmg += variables[0];
	nextCard->healing += variables[0];

	for(int i = 0;i < variables.size();i++)
	{
		nextCard->variables[i] += variables[0];
	}
}
