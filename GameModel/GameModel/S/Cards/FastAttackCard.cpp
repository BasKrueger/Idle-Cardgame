#include "H/FastAttackCard.h"
#include "H/DamageInteraction.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"
#include "H/PlayInteraction.h"

const int FastAttackCard::cardID;

void FastAttackCard::InternalInitialize(int& cID, int& baseDmg, int& baseHealing, int& baseCooldown, 
    std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName)
{
    cID = cardID;
    baseCooldown = 3;
    baseDmg = 1;

    iconName = "Set1/Bow";
    cardName->SetKey("FastAttack_Name");
    cardDescription->SetKey("FastAttack_Text");
    cardDescription->BindFormatVariable(&dmg);
}

void FastAttackCard::Play(PlayInteraction* play)
{
    auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, play->pTarget);
    interaction->Initialize(BaseCard::dmg);
    InteractionManager::AddNext(interaction);
}