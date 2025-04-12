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
    InternalInitialize(cardID, baseDmg, baseHealing, baseCooldown, baseVariables, cardName, cardDescription, iconName);

    Reset();
}

void BaseCard::Reset()
{
    cooldown = baseCooldown;
    dmg = baseDmg;
    healing = baseHealing;
    
    for (int i = 0; i < baseVariables.size(); i++) 
    {
        variables[i] = baseVariables[i];
    }
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

void BaseCard::Play(PlayInteraction* pTarget) {}
void BaseCard::InternalInitialize(int& cardID, int& baseDmg, int& baseHealing, int& baseCooldown, 
    std::array<int, 3>& baseVariables, LocalizedString* cardName, LocalizedString* cardDescription, std::string& iconName) {}

#pragma region State/Save/Load
json::JSON* BaseCard::GetState()
{
    if (state == nullptr) state = new json::JSON;

    (*state)["id"] = id;
    (*state)["dmg"] = dmg;
    (*state)["activeCooldown"] = cooldown;
    (*state)["cooldown"] = baseCooldown;
    (*state)["cardIcon"] = iconName;
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

#include "H/AttackCard.h"
BaseCard* BaseCard::LoadSave(Character* owner, json::JSON save)
{
    auto card = CardPool::GetInstance(owner, save["cardID"].ToInt());
    card->dmg = save["dmg"].ToInt();
    card->cooldown = save["activeCooldown"].ToInt();

    for (auto i = 0; i < save["variables"].size(); i++) 
    {
        card->variables[i] = save["variables"][i].ToInt();
    }

    return card;
}
#pragma endregion