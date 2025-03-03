#pragma once
#define INTERACTION(T)	inline virtual void Before(T* interaction){}\
						inline virtual void After(T* interaction){}
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
	
	INTERACTION(DieInteraction)
	INTERACTION(PlayInteraction)
	INTERACTION(DamageInteraction)
};