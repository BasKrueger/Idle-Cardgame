#include "pch.h"
#include "FastAttackCard.h"
#include "DamageInteraction.h"
#include "InteractionManager.h"
#include "InteractionPool.h"
#include "PlayInteraction.h"
#include "CardPool.h"

void FastAttackCard::InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription)
{
    baseCooldown = 3;
    baseDmg = 1;

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

void FastAttackCard::ReturnToPool()
{
    CardPool<FastAttackCard>().ReturnInstance(this);
}
