#pragma once
#include "BaseInteraction.h"
#include <Vector>
#include "InterActor.h"
#include "InteractionPool.h"

template <typename T>
class BeforeInteraction : public BaseInteraction
{
	T* pInteraction;
	std::vector<InterActor*> actors;

public:
	void Initialize(T* interaction, std::vector<InterActor*> pActors);

protected:
	void ReturnToPool() override;
	void InternalPerform() override;
};

template<typename T>
inline void BeforeInteraction<T>::Initialize(T* interaction, std::vector<InterActor*> pActors)
{
	pInteraction = interaction;
	actors = pActors;
}

template<typename T>
inline void BeforeInteraction<T>::InternalPerform()
{
	for (auto actor : actors)  
	{
		if (actor == 0) continue;

		actor->Before(pInteraction);  
	}
}

template<typename T>
inline void BeforeInteraction<T>::ReturnToPool()
{
	InteractionPool<BeforeInteraction<T>>().ReturnInstance(this);
}

