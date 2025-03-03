#pragma once
#include <vector>
#include <iostream>

template <typename T>
class GenericPool
{
public:
	static T* GetInstance();
	static void ReturnInstance(T* pInstance);

private:
	static std::vector<T*> poolContent;
};

template<typename T>
std::vector<T*> GenericPool<T>::poolContent;

template<typename T>
inline T* GenericPool<T>::GetInstance()
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

	return result;
}

template<typename T>
inline void GenericPool<T>::ReturnInstance(T* pInstance)
{
	poolContent.push_back(pInstance);
}
