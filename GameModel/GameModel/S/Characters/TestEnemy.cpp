#include "H/TestEnemy.h"
#include "H/AttackCard.h"
#include "H/CardPool.h"
#include "H/CharacterPool.h"

const int TestEnemy::characterID;

void TestEnemy::InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName)
{
	charID = characterID;
	baseHP = 5;
	baseDamage = 0;
	characterName->SetKey("GenericEnemy");

	for (int i = 0; i < 3; i++)
	{
		deck[i] = CardPool::GetInstance(this, AttackCard::cardID);
	}
}