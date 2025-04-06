#pragma once
#define INTERACT(T)	virtual void Before(T* interaction){}\
					virtual void After(T* interaction){}

class Character;

class DamageInteraction;
class DieInteraction;
class PlayInteraction;

class InterActor
{
public:
	InterActor();
	~InterActor();

	void Register();
	void UnRegister();

	int actorIndex = -1;
	Character* pOwner = nullptr;
	
	INTERACT(DieInteraction)
	INTERACT(PlayInteraction)
	INTERACT(DamageInteraction)
};