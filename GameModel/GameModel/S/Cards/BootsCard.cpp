#include "H/BootsCard.h"
#include "H/InteractionManager.h"

const int BootsCard::cardID;

void BootsCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 8;
	baseVariables[0] = 3;
	baseVariables[1] = 1;

	iconName = "Set2/Icon7";
	cardDescription->SetKey("Boots");
	cardDescription->BindFormatVariable(&variables[0]);
	cardDescription->BindFormatVariable(&variables[1]);
}

void BootsCard::Play(PlayInteraction* pPlay)
{
	for (int i = 0; i < variables[0]; i++) 
	{
		pPlay->pSource->pOwner->deck->GetNextCard(i + 1)->baseCooldown -= variables[1];
	}
}
