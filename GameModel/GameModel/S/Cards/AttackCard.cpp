#include "H/AttackCard.h"
#include "H/DamageInteraction.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"
#include "H/PlayInteraction.h"

const int AttackCard::cardID;

void AttackCard::InternalInitialize(int& cID,  int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription)
{
    cID = cardID;
    baseCooldown = 6;
    baseDmg = 2;

    cardName->SetKey("Attack_Name");
    cardDescription->SetKey("Attack_Text");
    cardDescription->BindFormatVariable(&dmg);
}

void AttackCard::Play(PlayInteraction* play)
{
    auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, play->pTarget);
    interaction->Initialize(BaseCard::dmg);
    InteractionManager::AddNext(interaction);
}