#include "H/SpikeShieldCard.h"
#include "H/InteractionManager.h"
#include "H/Character.h"
#include "H/InteractionPool.h"

const int SpikeShieldCard::cardID;

void SpikeShieldCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
	std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 1;
	iconName = "Set1/SpikeShield";

	cardName->SetKey("SpikeShield_Name");
	cardDescription->SetKey("SpikeShield_Text");
	cardDescription->BindFormatVariable(&variables[0]);
}

void SpikeShieldCard::Play(PlayInteraction* pPlay)
{
}

void SpikeShieldCard::Before(DamageInteraction* interaction)
{
	INITCHECK;

	if (pOwner->deck->GetNextCard(0) != this) return;
	if (interaction->pTarget->pOwner == this->pOwner) 
	{
		interaction->attackDamage -= variables[0];
		if (interaction->attackDamage <= 0) return;

		auto inter = InteractionPool<DamageInteraction>().GetInstance(this, interaction->pSource);
		inter->attackDamage = interaction->attackDamage;
		InteractionManager::AddNext(inter);
	}
}
