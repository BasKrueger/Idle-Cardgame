#include "H/CharacterPool.h"
#include "H/Player.h"
#include "H/TestEnemy.h"

#define RegCharacter(CharacterClassName) case CharacterClassName::characterID: \
poolContent[CharacterClassName::characterID].push_back(new CharacterClassName()); \
break; \

std::map<int, std::vector<Character*>> CharacterPool::poolContent;

void CharacterPool::CreateInstance(int characterID)
{
	switch (characterID)
	{
		RegCharacter(Player);
		RegCharacter(TestEnemy);
	}
}

Character* CharacterPool::GetInstance(int characterID)
{
	if (!poolContent.count(characterID))
	{
		CreateInstance(characterID);
	}

	if (poolContent[characterID].size() == 0)
	{
		CreateInstance(characterID);
	}

	Character* result = poolContent[characterID].back();
	poolContent[characterID].pop_back();

	result->Initialize();
	result->Register();

	return result;
}

void CharacterPool::ReturnInstance(Character* pInstance)
{
	poolContent[pInstance->characterID].push_back(pInstance);
}
