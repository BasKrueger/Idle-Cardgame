#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "Character.h"

#define RegCharacter(CharacterClassName) case CharacterClassName::encounterID: \
poolContent[CharacterClassName::encounterID].push_back(new CharacterClassName()); \
break; \

class Character;

class CharacterPool
{
public:
	static Character* GetInstance(int characterID);
	static void ReturnInstance(Character* pInstance);

private:
	static std::map<int, std::vector<Character*>> poolContent;
	static void CreateInstance(int encounterID);
};