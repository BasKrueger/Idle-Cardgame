#include "H/DoubleStrikeCard.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"

const int DoubleStrikeCard::cardID;

void DoubleStrikeCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseDmg = 1;
	baseCooldown = 7;
	iconName = "Set1/Wand";

	baseVariables[0] = 2;
	cardName->SetKey("DoubleStrike_Name");
	cardDescription->SetKey("DoubleStrike_Text");
	cardDescription->BindFormatVariable(&dmg);
	cardDescription->BindFormatVariable(&variables[0]);
}

void DoubleStrikeCard::Play(PlayInteraction* pPlay)
{
	for (int i = 0; i < variables[0]; i++) 
	{
		auto interaction = InteractionPool<DamageInteraction>().GetInstance(pPlay);
		interaction->Initialize(dmg);
		InteractionManager::AddNext(interaction);
	}
}
