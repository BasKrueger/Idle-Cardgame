#include "H/CrossSwords.h"
#include "H/InteractionManager.h"

const int CrossSwords::cardID;

void CrossSwords::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;

	iconName = "Set2/Icon14";
	cardName->SetKey("CrossSwords_Name");
	cardDescription->SetKey("CrossSwords_Text");
	cardDescription->BindFormatVariable(&dmg);
}

void CrossSwords::Before(PlayInteraction* interaction)
{
	INITCHECK

	pOwner->deck->FindCardIndex(this, variables[0]);
	dmg = pOwner->deck->GetCardByIndex(variables[0] + 1)->dmg + pOwner->deck->GetCardByIndex(variables[0] - 1)->dmg;
}

void CrossSwords::Play(PlayInteraction* pPlay)
{
	auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, pPlay->pTarget);
	interaction->Initialize(BaseCard::dmg);
	InteractionManager::AddNext(interaction);
}
