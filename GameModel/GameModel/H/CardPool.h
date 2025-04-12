#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <array>

class BaseCard;
class Character;

class CardPool
{
public:
	static void Initialize();
	static BaseCard* GetInstance(Character* owner, int cardID);
	static void ReturnInstance(BaseCard* pInstance);
	static bool ReFillPool(int cardID);
	static int maxCardID;

private:
	static std::map<int, std::array<BaseCard*, 10>> poolContent;
};

