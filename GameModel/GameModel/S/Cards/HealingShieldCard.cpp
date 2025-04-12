#include "H/HealingShieldCard.h"
#include "H/InteractionManager.h"
#include "H/Character.h"

const int HealingShieldCard::cardID;

void HealingShieldCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 6;
	iconName = "Set2/Icon10";

	cardName->SetKey("HealingShield_Name");
	cardDescription->SetKey("HealingShield_Text");
}

void HealingShieldCard::Play(PlayInteraction* pPlay)
{
}

void HealingShieldCard::Before(DamageInteraction* interaction)
{
	INITCHECK

	if (this->pOwner->deck->GetNextCard(0) != this) return;
	if (interaction->pTarget == this->pOwner && interaction->attackDamage > 0)
	{
		auto heal = InteractionPool<HealInteraction>().GetInstance(this->pOwner, this->pOwner);
		heal->healing = interaction->attackDamage;
		interaction->attackDamage = 0;
		
		InteractionManager::AddNext(heal);
	}
}
