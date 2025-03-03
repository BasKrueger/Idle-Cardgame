#pragma once
#include <vector>
#include <iostream>
#include "BaseCard.h"

class BaseCard;

template <typename T>
class CardPool
{
public:
	static T* GetInstance(Character* owner);
	static void ReturnInstance(T* pInstance);

private:
	static std::vector<T*> poolContent;
};

template<typename T>
std::vector<T*> CardPool<T>::poolContent;

template<typename T>
inline T* CardPool<T>::GetInstance(Character* owner)
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
		result->Initialize();
	}

	if (result == 0)
	{
		std::cout << "Error while getting a pooled object instance\n";
	}

	result->Reset();
	result->Register();
	result->pOwner = owner;

	return result;
}

template<typename T>
inline void CardPool<T>::ReturnInstance(T* pInstance)
{
	pInstance->UnRegister();
	poolContent.push_back(pInstance);
}
