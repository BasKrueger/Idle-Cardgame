#include "H/BaseCard.h"
#include "H/InteractionPool.h"
#include "H/InteractionManager.h"
#include "H/Character.h"
#include "H/PlayInteraction.h"
#include "H/CardPool.h"
#include "H/AddBuffInteraction.h"
#include "H/RemoveBuffInteraction.h"
#include "H/Buff.h"
#include "H/BuffPool.h"

void BaseCard::Initialize()
{
    cardName = new LocalizedString(LocalizedString::TABLE::CARDS);
    cardDescription = new LocalizedString(LocalizedString::TABLE::CARDS);
    InternalInitialize(cardID, baseDmg, baseHealing, baseCooldown, baseVariables, cardName, cardDescription, iconName);

    FullReset();
}

void BaseCard::ResetCooldown()
{
    cooldown = baseCooldown;
}

void BaseCard::FullReset()
{
    cooldown = baseCooldown;
    dmg = baseDmg;
    healing = baseHealing;

    for (int i = 0; i < baseVariables.size(); i++)
    {
        variables[i] = baseVariables[i];
    }

    for (int i = 0; i < buffs.size(); i++)
    {
        if (buffs[i] == nullptr) continue;
        BuffPool::ReturnInstance(buffs[i]);
        buffs[i] = nullptr;
    }
}

void BaseCard::Tick()
{
    cooldown -= 1;
}

void BaseCard::BuffTick()
{
    for (auto& buff : buffs)
    {
        if (buff == nullptr) continue;
        buff->Tick();
    }
}

bool BaseCard::IsCharged()
{
    return cooldown < 0;
}

void BaseCard::ReturnToPool()
{
    CardPool::ReturnInstance(this);
}

void BaseCard::AttachBuff(AddBuffInteraction* pInteraction)
{
    for(int i = 0;i < buffs.size();i++)
    {
        if (buffs[i] == nullptr) 
        {
            buffs[i] = pInteraction->buff;
            pInteraction->buff->pOwner = this->pOwner;
            pInteraction->buff->pCard = this;
            pInteraction->buff->OnApply();
            return;
        }
    }

    buffs.push_back(pInteraction->buff);
    pInteraction->buff->pOwner = this->pOwner;
    pInteraction->buff->pCard = this;
    pInteraction->buff->OnApply();
}

void BaseCard::RemoveBuff(RemoveBuffInteraction* pInteraction)
{
    for (int i = 0; i < buffs.size(); i++)
    {
        if (buffs[i] == pInteraction->buff)
        {
            pInteraction->buff->OnRemove();
            BuffPool::ReturnInstance(pInteraction->buff);
            buffs[i] = nullptr;
            break;
        }
    }
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

    (*state)["buffs"] = json::Array();
    for (auto& buff : buffs)
    {
        if (buff == nullptr) continue;
        (*state)["buffs"].append(*buff->GetState());
    }

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

    save["buffs"] = json::Array();
    for (auto& buff : buffs)
    {
        if (buff == nullptr) continue;
        save["buffs"].append(buff->GetSave());
    }

    return save;
}

BaseCard* BaseCard::LoadSave(Character* owner, json::JSON save)
{
    auto card = CardPool::GetInstance(owner, save["cardID"].ToInt());
    card->dmg = save["dmg"].ToInt();
    card->cooldown = save["activeCooldown"].ToInt();

    for (auto i = 0; i < save["variables"].size(); i++) 
    {
        card->variables[i] = save["variables"][i].ToInt();
    }

    for (auto i = 0; i < save["buffs"].size(); i++)
    {
        auto buff = Buff::LoadSave(card, save["buffs"][i]);
        card->buffs.push_back(buff);
    }

    return card;
}
#pragma endregion