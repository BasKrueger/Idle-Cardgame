#pragma once
#include <vector>
#include <iostream>
#include "PlayInteraction.h"

class InterActor;

template <typename T>
class InteractionPool
{
public:
	static T* GetInstance(InterActor* interactionSource, InterActor* interactionTarget);
	static T* GetInstance(BaseInteraction* pPlay);
	static void ReturnInstance(T* pInstance);

private:
	static std::vector<T*> poolContent;
};

template<typename T>
std::vector<T*> InteractionPool<T>::poolContent;

template<typename T>
inline T* InteractionPool<T>::GetInstance(InterActor* interactionSource, InterActor* interactionTarget)
{
	T* result = 0;

	if (poolContent.size() > 0)
	{
		result = poolContent.back();
		poolContent.pop_back();
	}
	else 
	{
		result = new T();
	}

	if (result == 0) 
	{
		std::cout << "Error while getting a pooled object instance\n";
	}

	result->BaseInitialization(interactionSource, interactionTarget);
	
	return result;
}

template<typename T>
inline T* InteractionPool<T>::GetInstance(BaseInteraction* pPlay)
{
	return GetInstance(pPlay->pSource, pPlay->pTarget);
}

template<typename T>
inline void InteractionPool<T>::ReturnInstance(T* pInstance)
{
	poolContent.push_back(pInstance);
}
