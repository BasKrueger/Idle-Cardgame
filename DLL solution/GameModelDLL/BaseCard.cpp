#include "pch.h"
#include "BaseCard.h"
#include "InteractionPool.h"
#include "InteractionManager.h"
#include "Character.h"
#include "PlayInteraction.h"

void BaseCard::Initialize()
{
    cardName = new LocalizedString(LocalizedString::TABLE::CARDS);
    cardDescription = new LocalizedString(LocalizedString::TABLE::CARDS);
    InternalInitialize(baseDmg, baseCooldown, cardName, cardDescription);
}

void BaseCard::Reset()
{
    cooldown = baseCooldown;
    dmg = baseDmg;
}

void BaseCard::Tick()
{
    cooldown -= 1;
}

bool BaseCard::IsCharged()
{
    return cooldown < 0;
}

bool BaseCard::TryPlay(InterActor* pTarget)
{
    if (IsCharged())
    {
        Reset();
        auto interaction = InteractionPool<PlayInteraction>().GetInstance(pOwner, pTarget);
        interaction->Initialize(this);
        InteractionManager::AddNext(interaction);
        return true;
    }

    return false;
}

void BaseCard::InternalInitialize(int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription){}
void BaseCard::Play(PlayInteraction* pTarget){}
void BaseCard::ReturnToPool(){}

json::JSON* BaseCard::GetState()
{
    if (state == 0) state = std::make_unique<json::JSON>();

    (*state)["id"] = id;
    (*state)["dmg"] = dmg;
    (*state)["activeCooldown"] = cooldown;
    (*state)["cooldown"] = baseCooldown;
    (*state)["cardName"] = cardName->Format();
    (*state)["cardDescription"] = cardDescription->Format();

    return state.get();
}



