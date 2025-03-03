#include "pch.h"
#include "AttackCard.h"
#include "DamageInteraction.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "PlayInteraction.h"
#include "CardPool.h"

void AttackCard::InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription)
{
    baseCooldown = 6;
    baseDmg = 1;

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

void AttackCard::ReturnToPool()
{
    CardPool<AttackCard>().ReturnInstance(this);
}