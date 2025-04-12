#pragma once
#include "BaseInteraction.h"
#include "InterActor.h"
#include "InteractionPool.h"
#include <Vector>

template <typename T>
class AfterInteraction : public BaseInteraction
{
	T* pInteraction = nullptr;
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
		if (actor == nullptr) continue;
		actor->After(pInteraction);
	}
}

template<typename T>
inline void AfterInteraction<T>::ReturnToPool()
{
	InteractionPool<AfterInteraction<T>>().ReturnInstance(this);
}
