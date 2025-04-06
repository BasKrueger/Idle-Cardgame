#include "H/BaseCard.h"
#include "H/InteractionPool.h"
#include "H/InteractionManager.h"
#include "H/Character.h"
#include "H/PlayInteraction.h"
#include "H/CardPool.h"

void BaseCard::Initialize()
{
    cardName = new LocalizedString(LocalizedString::TABLE::CARDS);
    cardDescription = new LocalizedString(LocalizedString::TABLE::CARDS);
    InternalInitialize(cardID, baseDmg, baseCooldown, cardName, cardDescription);

    Reset();
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

void BaseCard::ReturnToPool()
{
    CardPool::ReturnInstance(this);
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

void BaseCard::InternalInitialize(int& cardID, int& baseDmg, int& baseCooldown, LocalizedString* cardName, LocalizedString* cardDescription) {}
void BaseCard::Play(PlayInteraction* pTarget) {}

#pragma region State/Save/Load
json::JSON* BaseCard::GetState()
{
    if (state == nullptr) state = new json::JSON;

    (*state)["id"] = id;
    (*state)["dmg"] = dmg;
    (*state)["activeCooldown"] = cooldown;
    (*state)["cooldown"] = baseCooldown;
    (*state)["cardName"] = cardName->Format();
    (*state)["cardDescription"] = cardDescription->Format();

    return state;
}

json::JSON BaseCard::GetSave()
{
    json::JSON save;

    save["cardID"] = cardID;
    save["dmg"] = dmg;
    save["activeCooldown"] = cooldown;

    save["variables"] = json::Array();
    for (auto& var : variables) 
    {
        save["variables"].append(var);
    }

    return save;
}

BaseCard* BaseCard::LoadSave(Character* owner, json::JSON save)
{
    auto card = CardPool::GetInstance(owner, save["cardID"].ToInt());
    card->dmg = save["dmg"].ToInt();
    card->cooldown = save["activeCooldown"].ToInt();

    int iterator = 0;
    for (auto& var : save["variables"].ArrayRange()) 
    {
        card->variables[iterator] = var.ToInt();
        iterator++;
    }

    return card;
}
#pragma endregion