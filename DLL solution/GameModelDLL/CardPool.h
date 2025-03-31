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
	static BaseCard* GetInstance(Character* owner, int cardID);
	static void ReturnInstance(BaseCard* pInstance);
	static void ReFillPool(int cardID);

private:
	static std::map<int, std::array<BaseCard*, 50>> poolContent;
};

