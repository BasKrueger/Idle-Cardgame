#pragma once
#include "BaseInteraction.h"
#include "InterActor.h"
#include <vector>
#include <iostream>
#include "Character.h"

#include "InteractionPool.h"
#include "AfterInteraction.h"
#include "BeforeInteraction.h"
#include "DamageInteraction.h"
#include "DieInteraction.h"
#include "PlayInteraction.h"
#include "HealInteraction.h"
#include "PerformInteraction.h"
#include "AddBuffInteraction.h"
#include "RemoveBuffInteraction.h"

#define DECLARE_INTERACTIONHANDLING(T)\
private:\
	static std::vector<InterActor*> BeforeActors##T;\
	static std::vector<InterActor*> AfterActors##T;\
public:\
	static void AddLast(T* pInteraction);\
	static void AddNext(T* pInteraction);\
	static void ResolveNow(T* pInteraction);\
	\
	static void AddActorBefore##T(InterActor* pInterActor);\
	static void RemoveActorBefore##T(InterActor* pInterActor);\
	static void AddActorAfter##T(InterActor* pInterActor);\
	static void RemoveActorAfter##T(InterActor* pInterActor);



class InteractionManager
{
public:
	static void Initialize();

	DECLARE_INTERACTIONHANDLING(DamageInteraction)
	DECLARE_INTERACTIONHANDLING(DieInteraction)
	DECLARE_INTERACTIONHANDLING(PlayInteraction)
	DECLARE_INTERACTIONHANDLING(HealInteraction)
	DECLARE_INTERACTIONHANDLING(AddBuffInteraction)
	DECLARE_INTERACTIONHANDLING(RemoveBuffInteraction)

private:
	static std::vector<BaseInteraction*> queuedInteractions;
	static void Resolve();
};
