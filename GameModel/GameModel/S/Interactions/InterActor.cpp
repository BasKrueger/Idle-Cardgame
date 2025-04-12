#include "H/InterActor.h"
#include "H/InteractionManager.h"


#define DEFINE_INTERACTIONMETHODS(T, N)\
void InterActor::N(T* interaction)\
{\
	InteractionManager::RemoveActor##N##T(this);\
}

#define DEFINE_INTERACTIONRECEPTION(T)\
DEFINE_INTERACTIONMETHODS(T, Before)\
DEFINE_INTERACTIONMETHODS(T, After)

#define INITINTERACTION(T)\
{T* interaction = nullptr;\
InteractionManager::AddActorBefore##T(this);\
InteractionManager::AddActorAfter##T(this);\
Before(interaction);\
After(interaction);}

#define REMOVEINTERACTION(T)\
InteractionManager::RemoveActorBefore##T(this);\
InteractionManager::RemoveActorAfter##T(this);

DEFINE_INTERACTIONRECEPTION(DieInteraction)
DEFINE_INTERACTIONRECEPTION(PlayInteraction)
DEFINE_INTERACTIONRECEPTION(DamageInteraction)
DEFINE_INTERACTIONRECEPTION(HealInteraction)

InterActor::~InterActor()
{
	UnRegister();
}

void InterActor::Register()
{
	if (isRegistered) return;
	isRegistered = true;

	INITINTERACTION(DieInteraction);
	INITINTERACTION(PlayInteraction);
	INITINTERACTION(DamageInteraction)
	INITINTERACTION(HealInteraction)
}

void InterActor::UnRegister()
{
	if (!isRegistered) return;
	isRegistered = false;

	REMOVEINTERACTION(DieInteraction)
	REMOVEINTERACTION(PlayInteraction)
	REMOVEINTERACTION(DamageInteraction)
	REMOVEINTERACTION(HealInteraction)
}
