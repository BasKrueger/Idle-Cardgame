#include "pch.h"
#include "SlowAttackCard.h"
#include "DamageInteraction.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "PlayInteraction.h"

void SlowAttackCard::InternalInitialize(int& cID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription)
{
    cID = cardID;
    baseCooldown = 8;
    baseDmg = 3;

    cardName->SetKey("SlowAttack_Name");
    cardDescription->SetKey("SlowAttack_Text");
    cardDescription->BindFormatVariable(&dmg);
}

void SlowAttackCard::Play(PlayInteraction* play)
{
    auto interaction = InteractionPool<DamageInteraction>().GetInstance(this, play->pTarget);
    interaction->Initialize(BaseCard::dmg);
    InteractionManager::AddNext(interaction);
}