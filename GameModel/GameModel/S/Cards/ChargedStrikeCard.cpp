#include "H/ChargedStrikeCard.h"
#include "H/InteractionPool.h"
#include "H/DamageInteraction.h"
#include "H/InteractionManager.h"

const int ChargedStrikeCard::cardID;

void ChargedStrikeCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 1;

	iconName = "Set2/Icon11";
	cardName->SetKey("ChargedStrike_Name");
	cardDescription->SetKey("ChargedStrike_Text");
	cardDescription->BindFormatVariable(&dmg);
	cardDescription->BindFormatVariable(&variables[0]);
}

void ChargedStrikeCard::Play(PlayInteraction* pPlay)
{
	auto interaction = InteractionPool<DamageInteraction>().GetInstance(pPlay);
	interaction->Initialize(dmg);
	InteractionManager::AddNext(interaction);
}

void ChargedStrikeCard::After(PlayInteraction* interaction)
{
	INITCHECK
	if (interaction->pSource != pOwner) return;
	dmg += variables[0];
}

