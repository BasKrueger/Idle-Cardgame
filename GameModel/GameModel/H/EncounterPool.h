#pragma once
#include <vector>
#include <iostream>
#include <map>

class Player;
class BaseEncounter;

class EncounterPool
{
public:
	static BaseEncounter* GetInstance(Player* pPlayer, int encounterID);
	static void ReturnInstance(BaseEncounter* pInstance);
	
private:
	static std::map<int, std::vector<BaseEncounter*>> poolContent;
	static void CreateInstance(int encounterID);
};