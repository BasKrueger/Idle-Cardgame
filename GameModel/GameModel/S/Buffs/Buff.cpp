#include "H/Buff.h"
#include "H/BaseCard.h"
#include "H/InteractionManager.h"
#include "H/InteractionPool.h"
#include "H/RemoveBuffInteraction.h"
#include "H/BuffPool.h"

void Buff::Initialize()
{
	InternalInitialize(buffID, duration, baseVariables, iconName);

	for (int i = 0; i < baseVariables.size(); i++)
	{
		variables[i] = baseVariables[i];
	}
}

void Buff::Tick()
{
	OnTick();

	duration -= 1;
	if (duration <= 0) 
	{
		auto removeInteraction = InteractionPool<RemoveBuffInteraction>().GetInstance(this, pCard);
		removeInteraction->Initialize(this, pCard);
		InteractionManager::AddNext(removeInteraction);
	}
}

void Buff::OnApply() {}
void Buff::OnRemove() {}
void Buff::OnTick() {}
void Buff::InternalInitialize(int& id, int& duration, std::array<int, 3>& variables, std::string& iconName) {}

#pragma region State/Save/Load

json::JSON* Buff::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["icon"] = iconName;
	(*state)["duration"] = duration;
	(*state)["id"] = id;

	return state;
}

json::JSON Buff::GetSave()
{
	json::JSON save;

	save["id"] = buffID;
	save["variables"] = json::Array();
	for (int i = 0; i < variables.size(); i++)
	{
		save["variables"].append(variables[i]);
	}

	return save;
}

void Buff::SetSave(json::JSON save)
{
	for (int i = 0; i < variables.size(); i++) 
	{
		variables[i] = save["variables"][i].ToInt();
	}
}

Buff* Buff::LoadSave(BaseCard* owner, json::JSON json)
{
	auto buff = BuffPool::GetInstance(json["id"].ToInt());
	buff->SetSave(json);
	buff->pOwner = owner->pOwner;
	buff->pCard = owner;

	return buff;
}

#pragma endregion