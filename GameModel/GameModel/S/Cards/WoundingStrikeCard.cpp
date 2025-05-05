#include "H/WoundingStrikeCard.h"
#include "H/InteractionPool.h"
#include "H/AddBuffInteraction.h"
#include "H/BuffPool.h"
#include "H/BleedBuff.h"
#include "H/Character.h"
#include "H/InteractionManager.h"

const int WoundingStrikeCard::cardID;

void WoundingStrikeCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 4;

	iconName = "Set2/Icon18";
	cardDescription->SetKey("WoundingStrike");
}

void WoundingStrikeCard::Play(PlayInteraction* pPlay)
{
	auto interaction = InteractionPool<AddBuffInteraction>().GetInstance(pPlay);
	auto buff = BuffPool::GetInstance(BleedBuff::buffID);
	interaction->Initialize(buff, pPlay->pSource->pOwner);
	InteractionManager::AddNext(interaction);
}

void WoundingStrikeCard::After(PlayInteraction* interaction)
{
	variables[1] = dmg * variables[0];
}
