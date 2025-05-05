#include "H/ThrowSwordCard.h"
#include "H/InteractionManager.h"

const int ThrowSwordCard::cardID;

void ThrowSwordCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown,
	std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
	cID = cardID;
	baseCooldown = 10;
	baseVariables[0] = 3;

	iconName = "Set2/Icon28";
	cardDescription->SetKey("ThrowSword");
	cardDescription->BindFormatVariable(&variables[0]);
}

void ThrowSwordCard::Play(PlayInteraction* pPlay)
{
	auto newPlay = InteractionPool<PlayInteraction>().GetInstance(pPlay);
	auto nextCard = pPlay->pSource->pOwner->deck->GetNextCard();
	nextCard->dmg *= variables[0];

	newPlay->Initialize(nextCard);
	
	InteractionManager::ResolveNow(newPlay);

	nextCard->dmg = 0;
	for (int i = 0; i < variables.size(); i++)
	{
		nextCard->variables[i] = 0;
	}
}
