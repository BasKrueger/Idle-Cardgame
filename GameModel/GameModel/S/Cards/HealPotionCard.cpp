#include "H/HealPotionCard.h"
#include "H/InteractionPool.h"
#include "H/HealInteraction.h"
#include "H/InteractionManager.h"

const int HealPotionCard::cardID;

void HealPotionCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseHealing = 15;

	iconName = "Set1/HPPotion";
	cardName->SetKey("HPPotion_Name");
	cardDescription->SetKey("HPPotion_Text");
	cardDescription->BindFormatVariable(&baseHealing);
}

void HealPotionCard::Play(PlayInteraction* play)
{
	auto interaction = InteractionPool<HealInteraction>().GetInstance(this, play->pTarget);
	interaction->Initialize(BaseCard::healing);
	InteractionManager::AddNext(interaction);
}
