#pragma once
#include <vector>
#include <iostream>
#include "Character.h"

class Character;

template <typename T>
class CharacterPool
{
public:
	static T* GetInstance();
	static void ReturnInstance(T* pInstance);

private:
	static std::vector<T*> poolContent;
};

template<typename T>
std::vector<T*> CharacterPool<T>::poolContent;

template<typename T>
inline T* CharacterPool<T>::GetInstance()
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

	result->Register();
	result->Initialize();

	return result;
}

template<typename T>
inline void CharacterPool<T>::ReturnInstance(T* pInstance)
{
	pInstance->UnRegister();
	poolContent.push_back(pInstance);
}
