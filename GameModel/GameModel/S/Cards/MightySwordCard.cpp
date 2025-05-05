#include "H/MightySwordCard.h"
#include "H/InteractionManager.h"

const int MightySwordCard::cardID;

void MightySwordCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 12;
	baseVariables[0] = 3;

	iconName = "Set2/Icon25";
	cardDescription->SetKey("MightySword");
	cardDescription->BindFormatVariable(&dmg);
	cardDescription->BindFormatVariable(&variables[0]);
	cardDescription->BindFormatVariable(&variables[1]);
}

void MightySwordCard::Play(PlayInteraction* pPlay)
{
	auto interaction = InteractionPool<DamageInteraction>().GetInstance(pPlay);
	interaction->Initialize(dmg * variables[0]);
	InteractionManager::AddNext(interaction);
}

void MightySwordCard::After(PlayInteraction* interaction)
{
	variables[1] = dmg * variables[0];
}
