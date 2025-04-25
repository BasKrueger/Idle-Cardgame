#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <array>

class Buff;
class Character;
class BaseCard;

class BuffPool
{
public:
	static void Initialize();
	static Buff* GetInstance(int buffID);
	static void ReturnInstance(Buff* pInstance);
	static bool ReFillPool(int buffID);

private:
	static std::map<int, std::array<Buff*, 10>> poolContent;
};

