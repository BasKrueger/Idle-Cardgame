#pragma once
#include <vector>
#include <iostream>

class Character;
class BsaeEncounter;

template <typename T>
class EncounterPool
{
public:
	static T* GetInstance(std::unique_ptr<Character>* pPlayer);
	static void ReturnInstance(T* pInstance);

private:
	static std::vector<T*> poolContent;
};

template<typename T>
std::vector<T*> EncounterPool<T>::poolContent;

template<typename T>
inline T* EncounterPool<T>::GetInstance(std::unique_ptr<Character>* pPlayer)
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

	result->BaseInitialize(pPlayer);

	return result;
}

template<typename T>
inline void EncounterPool<T>::ReturnInstance(T* pInstance)
{
	poolContent.push_back(pInstance);
}
