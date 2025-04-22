#include "H/Player.h"
#include "H/InteractionManager.h"
#include "H/CardPool.h"
#include "H/CharacterPool.h"
#include "H/DamageInteraction.h"
#include "H/InteractionPool.h"
#include "H/Json.hpp"

#include "H/AttackCard.h"
#include "H/FastAttackCard.h"
#include "H/SlowAttackCard.h"
#include "H/HealPotionCard.h"
#include "H/PowerPotionCard.h"
#include "H/DoubleStrikeCard.h"
#include "H/CrazyPotionCard.h"
#include "H/ChargedStrikeCard.h"
#include "H/SpikeShieldCard.h"
#include "H/HealingShieldCard.h"
#include "H/TieUpCard.h"

const int Player::characterID;

void Player::Before(DamageInteraction* interaction)
{
	INITCHECK

	if (interaction->pTarget != this) return;
	if(interaction->attackDamage >= hp) interaction->attackDamage = 0;
}

void Player::After(DieInteraction* interaction)
{
	INITCHECK

	if (interaction->pTarget != this) return;
	std::cout << "Player died\n";
}

void Player::AddToCollection(BaseCard* card)
{
	collection.Add(card);
}

void Player::InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName, Deck* deck)
{
	charID = characterID;
	baseHP = 100;
	baseDamage = 0;
	characterName->SetKey("Player");

	collection.Add(CardPool::GetInstance(this, FastAttackCard::cardID));
	collection.Add(CardPool::GetInstance(this, DoubleStrikeCard::cardID));

	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, FastAttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, SlowAttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, HealPotionCard::cardID));
	deck->Add(CardPool::GetInstance(this, PowerPotionCard::cardID));
	deck->Add(CardPool::GetInstance(this, DoubleStrikeCard::cardID));
	deck->Add(CardPool::GetInstance(this, CrazyPotionCard::cardID));
	deck->Add(CardPool::GetInstance(this, FastAttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, ChargedStrikeCard::cardID));
	deck->Add(CardPool::GetInstance(this, SpikeShieldCard::cardID));
	deck->Add(CardPool::GetInstance(this, HealingShieldCard::cardID));
	deck->Add(CardPool::GetInstance(this, TieUpCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
	deck->Add(CardPool::GetInstance(this, AttackCard::cardID));
}

void Player::SwitchCards(int ID1, int ID2)
{
	auto card1 = deck->GetCardByID(ID1);
	if (card1 == nullptr) card1 = collection.GetCardByID(ID1);
	if (card1 == nullptr) return;

	auto card2 = deck->GetCardByID(ID2);
	if (card2 == nullptr) card2 = collection.GetCardByID(ID2);
	if (card2 == nullptr) return;

	if (!deck->TrySwapCard(&collection, card1, card2)) return;

	card1->Reset();
	card2->Reset();

	if (collection.Contains(card1)) card1->UnRegister();
	if (collection.Contains(card2)) card2->UnRegister();
	if (deck->Contains(card1)) card1->Register();
	if (deck->Contains(card2)) card2->Register();
}

#pragma region State/Save/Load

json::JSON* Player::GetState()
{
	if (state == nullptr) state = new json::JSON;
	
	(*state)["generic"] = *Character::GetState();
	(*state)["xp"] = xp;
	(*state)["gold"] = gold;
	(*state)["collection"] = *collection.GetState();

	return state;
}

json::JSON Player::GetSave()
{
	json::JSON save;

	(save)["generic"] = Character::GetSave();
	(save)["xp"] = xp;
	(save)["gold"] = gold;
	(save)["collection"] = collection.GetSave();

	return save;
}

void Player::SetSave(json::JSON save)
{
	Character::SetSave(save["generic"]);
	
	xp = save["xp"].ToInt();
	gold = save["gold"].ToInt();

	collection.SetSave(save["collection"], this);
	collection.SetCardsRegistered(false);
}

#pragma endregion