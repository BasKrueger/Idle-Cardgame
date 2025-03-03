#include "pch.h"
#include "InterActor.h"
#include "InteractionManager.h"

InterActor::InterActor()
{
	Register();
}

InterActor::~InterActor()
{
	UnRegister();
}

void InterActor::Register()
{
	if (actorIndex != -1) return;

	actorIndex = InteractionManager::AddActor(this);
}

void InterActor::UnRegister()
{
	if (actorIndex == -1) return;

	InteractionManager::RemoveActor(actorIndex);

	actorIndex = -1;
}
