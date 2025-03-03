#include "pch.h"
#include "InteractionManager.h"

std::vector<BaseInteraction*> InteractionManager::queuedInteractions;
std::vector<InterActor*> InteractionManager::actors;

void InteractionManager::Initialize()
{
	queuedInteractions.clear();
	actors.clear();
}

#include "DamageInteraction.h"

bool resolving = false;
void InteractionManager::Resolve()
{
	if (resolving) return;

	resolving = true;

	while(queuedInteractions.size() > 0)
	{
		auto interaction = queuedInteractions.front();
		queuedInteractions.erase(queuedInteractions.begin());

		if (interaction->pSource->pOwner->IsAlive() && interaction->pTarget->pOwner->IsAlive())
		{
			interaction->Perform();
		}
	}

	resolving = false;
}

int InteractionManager::AddActor(InterActor* pActor)
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (InteractionManager::actors[i] == nullptr)
		{
			InteractionManager::actors[i] = pActor;

			return i;
		}
	}

	auto index = static_cast<int>(InteractionManager::actors.size());
	InteractionManager::actors.push_back(pActor);

	return index;
}

void InteractionManager::RemoveActor(int index)
{
	if (index < 0 || index + 1 > InteractionManager::actors.size()) return;

	actors[index] = 0;
}
