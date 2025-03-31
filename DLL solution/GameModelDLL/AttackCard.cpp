#include "pch.h"
#include "AttackCard.h"
#include "DamageInteraction.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "PlayInteraction.h"

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