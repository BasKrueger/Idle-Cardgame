#pragma once
#include "BaseInteraction.h"
#include "InterActor.h"
#include <vector>
#include <iostream>

class InteractionManager
{
public:
	static void Initialize();

	template <typename T>
	static void AddLast(T* pInteraction);

	template <typename T>
	static void AddNext(T* pInteraction);

	template <typename T>
	static void ResolveNow(T* pInteraction);
	
	static void RemoveActor(int index);
	static int AddActor(InterActor* actor);

private:
	static std::vector<InterActor*> actors;
	static std::vector<BaseInteraction*> queuedInteractions;
	static void Resolve();
};

#include "InteractionPool.h"
#include "AfterInteraction.h"
#include "BeforeInteraction.h"
#include "DamageInteraction.h"
#include "DieInteraction.h"
#include "PlayInteraction.h"

#include "PerformInteraction.h"

#include "Character.h"

template<typename T>
inline void InteractionManager::AddLast(T* pInteraction)
{
	auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	before->Initialize(pInteraction, actors);
	
	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	perform->Initialize(pInteraction);

	auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	after->Initialize(pInteraction, actors);

	queuedInteractions.push_back(before);
	queuedInteractions.push_back(perform);
	queuedInteractions.push_back(after);

	Resolve();
}

template<typename T>
inline void InteractionManager::AddNext(T* pInteraction)
{
	auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	before->Initialize(pInteraction, actors);

	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	perform->Initialize(pInteraction);

	auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	after->Initialize(pInteraction, actors);

	queuedInteractions.insert(queuedInteractions.begin(), after);
	queuedInteractions.insert(queuedInteractions.begin(), perform);
	queuedInteractions.insert(queuedInteractions.begin(), before);

	Resolve();
}

template<typename T>
inline void InteractionManager::ResolveNow(T* pInteraction)
{
	auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	before->Initialize(pInteraction, actors);

	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	perform->Initialize(pInteraction);

	auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);
	after->Initialize(pInteraction, actors);

	before->Perform();
	perform->Perform();
	after->Perform();
}
