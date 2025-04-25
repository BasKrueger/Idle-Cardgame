#pragma once
#include <vector>

#define INITCHECK if(interaction == nullptr) {return;}
#define DECLARE_INTERACTIONRECEPTION(T)	\
					virtual void Before(T* interaction);\
					virtual void After(T* interaction);

class Character;
class DamageInteraction;
class DieInteraction;
class PlayInteraction;
class HealInteraction;
class RemoveBuffInteraction;
class AddBuffInteraction;

class InterActor
{
public:
	~InterActor();

	void Register();
	void UnRegister();

	int actorIndex = -1;
	Character* pOwner = nullptr;

	DECLARE_INTERACTIONRECEPTION(DieInteraction)
	DECLARE_INTERACTIONRECEPTION(PlayInteraction)
	DECLARE_INTERACTIONRECEPTION(DamageInteraction)
	DECLARE_INTERACTIONRECEPTION(HealInteraction)
	DECLARE_INTERACTIONRECEPTION(RemoveBuffInteraction)
	DECLARE_INTERACTIONRECEPTION(AddBuffInteraction)

private:
	bool isRegistered = false;
};