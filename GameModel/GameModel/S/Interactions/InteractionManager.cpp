#include "H/InteractionManager.h"

std::vector<BaseInteraction*> InteractionManager::queuedInteractions;

#define DEFINE_ACTORADDITION(T, N) \
void InteractionManager::AddActor##N##T(InterActor* pInterActor) \
{ \
    auto& vec = InteractionManager::N##Actors##T; \
    for (int i = 0; i < vec.size(); i++) { \
        if (vec[i] == nullptr) \
        { \
            vec[i] = pInterActor; \
            return; \
        } \
    } \
    vec.push_back(pInterActor); \
}

#define DEFINE_ACTORREMOVAL(T, N) \
void InteractionManager::RemoveActor##N##T(InterActor* pInterActor) \
{ \
    auto& vec = InteractionManager::N##Actors##T; \
    for (int i = 0; i < vec.size(); i++) \
    { \
        if (vec[i] == pInterActor) { \
            vec[i] = nullptr; \
            return; \
        } \
    } \
}

#define DEFINE_ADDNEXT(T)\
void InteractionManager::AddNext(T* pInteraction)\
{\
	if(BeforeActors##T.size() > 0)\
	{\
		auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget); \
		before->Initialize(pInteraction, BeforeActors##T); \
		queuedInteractions.push_back(before); \
	}\
	\
	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);\
	perform->Initialize(pInteraction);\
	queuedInteractions.push_back(perform);\
	\
	if(AfterActors##T.size() > 0)\
	{\
		auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget); \
		after->Initialize(pInteraction, AfterActors##T); \
		queuedInteractions.push_back(after); \
	}\
	Resolve();\
}

#define DEFINE_ADDLAST(T)\
void InteractionManager::AddLast(T* pInteraction)\
{\
	if(BeforeActors##T##.size() > 0)\
	{\
		auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget); \
		before->Initialize(pInteraction, BeforeActors##T); \
		queuedInteractions.insert(queuedInteractions.begin(), before); \
	}\
	\
	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);\
	perform->Initialize(pInteraction);\
	queuedInteractions.insert(queuedInteractions.begin(), perform);\
	\
	if(AfterActors##T.size() > 0)\
	{\
		auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget); \
		after->Initialize(pInteraction, AfterActors##T); \
		queuedInteractions.insert(queuedInteractions.begin(), after); \
	}\
	Resolve();\
}

#define DEFINE_RESOLVENOW(T)\
void InteractionManager::ResolveNow(T* pInteraction)\
{\
	if(BeforeActors##T##.size() > 0) \
	{ \
		auto before = InteractionPool<BeforeInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget); \
		before->Initialize(pInteraction, BeforeActors##T##); \
		before->Perform(); \
	} \
	\
	auto perform = InteractionPool<PerformInteraction>().GetInstance(pInteraction->pSource, pInteraction->pTarget);\
	perform->Initialize(pInteraction);\
	perform->Perform();\
	\
	if(AfterActors##T##.size() > 0)\
	{\
		auto after = InteractionPool<AfterInteraction<T>>().GetInstance(pInteraction->pSource, pInteraction->pTarget);\
		after->Initialize(pInteraction, AfterActors##T##);\
		after->Perform();\
	}\
	Resolve();\
}

#define DEFINE_ACTORVECTORS(T)\
std::vector<InterActor*> InteractionManager::BeforeActors##T;\
std::vector<InterActor*> InteractionManager::AfterActors##T;

#define DEFINE_INTERACTIONHANDLING(T)\
DEFINE_ACTORVECTORS(T)\
DEFINE_ACTORADDITION(T, Before)\
DEFINE_ACTORADDITION(T, After)\
DEFINE_ACTORREMOVAL(T, Before)\
DEFINE_ACTORREMOVAL(T, After)\
DEFINE_ADDNEXT(T)\
DEFINE_ADDLAST(T)\
DEFINE_RESOLVENOW(T)

#define CLEARACTORS(T)\
	BeforeActors##T.clear();\
	AfterActors##T.clear();

bool resolving = false;
void InteractionManager::Initialize()
{
	CLEARACTORS(DamageInteraction)
	CLEARACTORS(DieInteraction)
	CLEARACTORS(PlayInteraction)
	CLEARACTORS(HealInteraction)

	queuedInteractions.clear();
}

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

DEFINE_INTERACTIONHANDLING(DamageInteraction)
DEFINE_INTERACTIONHANDLING(DieInteraction)
DEFINE_INTERACTIONHANDLING(PlayInteraction)
DEFINE_INTERACTIONHANDLING(HealInteraction)
DEFINE_INTERACTIONHANDLING(AddBuffInteraction)
DEFINE_INTERACTIONHANDLING(RemoveBuffInteraction)


