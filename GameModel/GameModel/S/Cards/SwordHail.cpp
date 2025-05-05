#include "H/SwordHail.h"
#include "H/InteractionManager.h"

const int SwordHail::cardID;

void SwordHail::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 30;
	baseVariables[0] = 8;
	baseDmg = 1;

	iconName = "Set2/Icon20";
	cardDescription->SetKey("SwordHail");
	cardDescription->BindFormatVariable(&dmg);
	cardDescription->BindFormatVariable(&variables[0]);
}

void SwordHail::Play(PlayInteraction* pPlay)
{
	for (int i = 0; i < variables[0]; i++)
	{
		auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, pPlay->pTarget);
		interaction->Initialize(BaseCard::dmg);
		InteractionManager::AddNext(interaction);
	}
}
