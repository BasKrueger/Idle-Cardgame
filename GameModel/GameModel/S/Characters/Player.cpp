#include "H/Player.h"
#include "H/AttackCard.h"
#include "H/InteractionManager.h"
#include "H/CardPool.h"
#include "H/CharacterPool.h"
#include "H/DamageInteraction.h"
#include "H/InteractionPool.h"
#include "H/Json.hpp"

const int Player::characterID;

void Player::Before(DamageInteraction* interaction)
{
	//testing only
	if (interaction->pTarget != this || hp > 98) return;
	interaction->attackDamage = 0;
}

void Player::AddToCollection(BaseCard* card)
{
	collection.push_back(card);
}

void Player::InternalInitialize(int& charID, int& baseHP, int& baseDamage, LocalizedString* characterName)
{
	charID = characterID;
	baseHP = 100;
	baseDamage = 0;
	characterName->SetKey("Player");

	for (int i = 0; i < 20; i++)
	{
		deck[i] = CardPool::GetInstance(this, AttackCard::cardID);
	}
	CardPool::ReFillPool(AttackCard::cardID);
}

int Player::SwitchCards(int collectionID, int deckID)
{
	int collectionIndex = -1;
	for(int i = 0;i < collection.size(); i++)
	{
		if(collection[i]->id == collectionID)
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
		if (deck[i]->id == deckID)
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

	return 0;
}

#pragma region State/Save/Load

json::JSON* Player::GetState()
{
	if (state == nullptr) state = new json::JSON;
	
	(*state)["generic"] = *Character::GetState();
	(*state)["xp"] = xp;
	(*state)["gold"] = gold;
	
	(*state)["collection"] = json::Array();
	for(auto& card : collection)
	{
		(*state)["collection"].append(*card->GetState());
	}

	return state;
}

json::JSON Player::GetSave()
{
	json::JSON save;

	(save)["generic"] = Character::GetSave();
	(save)["xp"] = xp;
	(save)["gold"] = gold;

	(save)["collection"] = json::Array();
	for (auto& card : collection)
	{
		(save)["collection"].append(card->GetSave());
	}

	return save;
}

void Player::SetSave(json::JSON save)
{
	Character::SetSave(save["generic"]);
	
	xp = save["xp"].ToInt();
	gold = save["gold"].ToInt();

	for (auto& card : collection) 
	{
		if (card == nullptr) continue;
		card->ReturnToPool();
	}

	collection.clear();

	for (auto& card : save["collection"].ArrayRange()) 
	{
		collection.push_back(BaseCard::LoadSave(this, card));
	}
}

#pragma endregion