#include "H/BreakSword.h"
#include "H/InteractionManager.h"

const int BreakSword::cardID;

void BreakSword::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 0;

	iconName = "Set2/Icon47";
	cardDescription->SetKey("BreakSword");
	cardDescription->BindFormatVariable(&variables[0]);
}

void BreakSword::Play(PlayInteraction* pPlay)
{
	pPlay->pTarget->pOwner->deck->GetNextCard(0)->dmg = 0;
}
