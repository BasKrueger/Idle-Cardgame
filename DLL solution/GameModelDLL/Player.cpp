#include "pch.h"
#include "Player.h"
#include "AttackCard.h"
#include "InteractionManager.h"
#include "CardPool.h"
#include "CharacterPool.h"
#include "DamageInteraction.h"
#include "InteractionPool.h"
#include "Json.hpp"

void Player::InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName)
{
	baseHP = 2147483647;
	baseDamage = 0;
	characterName->SetKey("Player");

	for (int i = 0; i < 20; i++)
	{
		deck[i] = CardPool<AttackCard>().GetInstance(this);
	}

	for (int i = 0; i < 3; i++)
	{
		auto card = CardPool<AttackCard>().GetInstance(this);
		card->UnRegister();
		collection.push_back(card);
	}
}

void Player::InternalReturnToPool()
{
	CharacterPool<Player>().ReturnInstance(this);
}

int Player::SwitchCards(int collectionID, int deckID)
{
	int collectionIndex = -1;
	for(int i = 0;i < collection.size(); i++)
	{
		if((int)collection[i] == collectionID)
		{
			collectionIndex = i;
			break;
		}
	}
	if (collectionIndex == -1) {
		std::cout << "Error: card not found in collection\n";
		return collectionID;
	}

	int deckIndex = -1;
	for (int i = 0; i < deck.size(); i++)
	{
		if ((int)deck[i] == deckID)
		{
			deckIndex = i;
			break;
		}
	}
	if (deckIndex == -1) {
		std::cout << "Error: card not found in deck\n";
		return deckID;
	}

	auto deckCard = deck[deckIndex];
	auto collectionCard = collection[collectionIndex];

	deck[deckIndex] = collectionCard;
	collection[collectionIndex] = deckCard;

	collectionCard->Register();
	collectionCard->Reset();
	deckCard->UnRegister();
	deckCard->Reset();

	std::cout << "\n\n\n\n\nSucesfully swapped " << collectionID << " with " << deckID << "\n\n\n\n\n";

	return 0;
}

json::JSON* Player::GetState()
{
	if (state == 0) state = std::make_unique<json::JSON>();
	
	(*state)["generic"] = *Character::GetState();
	(*state)["collection"] = json::Array();
	
	for(auto& card : collection)
	{
		(*state)["collection"].append(*card->GetState());
	}

	return state.get();
}

void Player::Before(DamageInteraction* interaction)
{
	if (interaction->pTarget != this) return;
	interaction->attackDamage = 0;
}