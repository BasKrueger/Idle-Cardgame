#include "pch.h"
#include "TestEnemy.h"
#include "AttackCard.h"
#include "CardPool.h"
#include "CharacterPool.h"

void TestEnemy::InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName)
{
	baseHP = 5;
	baseDamage = 0;
	characterName->SetKey("GenericEnemy");

	for (int i = 0; i < 3; i++)
	{
		deck[i] = CardPool<AttackCard>().GetInstance(this);
	}
}

void TestEnemy::InternalReturnToPool()
{
	CharacterPool<TestEnemy>().ReturnInstance(this);
}
