#include "H/SlowAttackCard.h"
#include "H/DamageInteraction.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"
#include "H/PlayInteraction.h"

const int SlowAttackCard::cardID;

void SlowAttackCard::InternalInitialize(int& cID, int& baseDmg, int&baseHealing, int& baseCooldown, 
    std::array<int, 3>& baseVariables, LocalizedString* cardDescription, std::string& iconName)
{
    cID = cardID;
    baseCooldown = 8;
    baseDmg = 3;

    iconName = "Set1/Axe";
    cardDescription->SetKey("SlowAttack");
    cardDescription->BindFormatVariable(&dmg);
}

void SlowAttackCard::Play(PlayInteraction* play)
{
    auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, play->pTarget);
    interaction->Initialize(BaseCard::dmg);
    InteractionManager::AddNext(interaction);
}