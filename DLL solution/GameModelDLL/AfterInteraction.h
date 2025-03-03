#pragma once
#include "BaseInteraction.h"
#include <Vector>
#include "InterActor.h"
#include "InteractionPool.h"

template <typename T>
class AfterInteraction : public BaseInteraction
{
	T* pInteraction = 0;
	std::vector<InterActor*> actors;;

public:
	void Initialize(T* interaction, std::vector<InterActor*> pActors);
	
protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};

template<typename T>
inline void AfterInteraction<T>::Initialize(T* interaction, std::vector<InterActor*> pActors)
{
	pInteraction = interaction;
	actors = pActors;
}

template<typename T>
inline void AfterInteraction<T>::InternalPerform()
{
	for (auto actor : actors)
	{
		if (actor == 0) continue;

		actor->After(pInteraction);
	}
}

template<typename T>
inline void AfterInteraction<T>::ReturnToPool()
{
	InteractionPool<AfterInteraction<T>>().ReturnInstance(this);
}
