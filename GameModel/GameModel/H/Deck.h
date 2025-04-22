#pragma once
#include "Json.hpp"
#include <Vector>

class BaseCard;
class Character;

class Deck
{
public:
	void Add(BaseCard* pCard);
	void Remove(BaseCard* pCard);
	bool TrySwapCard(Deck* pOtherDeck, BaseCard* pCard1, BaseCard* pCard2);

	void AdvanceToNextCard();
	void ReturnContentToPool();
	void SetCardsRegistered(bool active);

private:
	int currentCardIndex = 0;
	std::vector<BaseCard*> content;

#pragma region Utility
public:
	BaseCard* GetNextCard(int add = 1);
	BaseCard* GetCurrentCard();
	BaseCard* GetCardByID(int id);
	bool Contains(BaseCard* pCard);
	bool FindCardIndex(BaseCard* pCard, int& outVal);

private:
	void PushBackCards(int entryIndex);
#pragma endregion

#pragma region State/Save/Load
public:
	json::JSON* GetState();
	json::JSON GetSave();
	void SetSave(json::JSON save, Character* pCardOwner);

private:
	json::JSON* state = nullptr;
#pragma endregion

};

