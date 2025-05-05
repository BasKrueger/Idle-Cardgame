#include "H/HarmlessPotion.h"
#include "H/InteractionManager.h"

const int HarmlessPotion::cardID;

void HarmlessPotion::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 3;

	iconName = "Set1/PurplePotion";
	cardDescription->SetKey("HarmlessPotion");
	cardDescription->BindFormatVariable(&variables[0]);
}

void HarmlessPotion::Play(PlayInteraction* pPlay)
{
	auto nextCard = pPlay->pSource->pOwner->deck->GetNextCard();

	for (int i = 0; i < variables.size(); i++)
	{
		nextCard->variables[i] += variables[0];
	}
}
