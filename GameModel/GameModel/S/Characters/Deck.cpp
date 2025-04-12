#include "H/Deck.h"
#include "H/BaseCard.h"
#include "H/Character.h"

void Deck::Add(BaseCard* pCard)
{
	content.push_back(pCard);
}

void Deck::Remove(BaseCard* pCard)
{
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] == pCard)
		{
			content[i] = nullptr;
			PushBackCards(i);
			content.pop_back();
			return;
		}
	}
}

bool Deck::TrySwapCard(Deck* pOtherDeck, BaseCard* pCard1, BaseCard* pCard2)
{
	int card1Index = -1;
	bool card1IsInOtherDeck = true;
	if (FindCardIndex(pCard1, card1Index)) card1IsInOtherDeck = false;
	else if (pOtherDeck->FindCardIndex(pCard1, card1Index)){}
	else return false;

	int card2Index = -1;
	bool card2IsInOtherDeck = true;
	if (FindCardIndex(pCard2, card2Index)) card2IsInOtherDeck = false;
	else if (pOtherDeck->FindCardIndex(pCard2, card2Index)){}
	else return false;

	if (card1IsInOtherDeck) pOtherDeck->content[card2Index] = pCard1;
	else content[card2Index] = pCard1;
	if (card2IsInOtherDeck) pOtherDeck->content[card1Index] = pCard2;
	else content[card1Index] = pCard2;

	return true;
}

void Deck::AdvanceToNextCard()
{
	currentCardIndex++;
	if (currentCardIndex > content.size() - 1) currentCardIndex = 0;
}

void Deck::ReturnContentToPool()
{
	for(int i = 0;i < content.size(); i++)
	{
		if (content[i] == nullptr) continue;
		content[i]->UnRegister();
		delete content[i];
		content[i] = nullptr;
	}
	content.clear();
}

void Deck::SetCardsRegistered(bool active)
{
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] == nullptr) continue;
		if (active) content[i]->Register();
		else content[i]->UnRegister();
	}
}

#pragma region Utility
void Deck::PushBackCards(int entryIndex)
{
	for (int i = entryIndex; i < content.size() - 2; i++)
	{
		content[i] = content[i + 1];
	}
}

bool Deck::FindCardIndex(BaseCard* pCard, int& outVal)
{	
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] == nullptr) return false;
		if (content[i] == pCard) { outVal = i; return true; }
	}

	return false;
}

bool Deck::Contains(BaseCard* pCard)
{
	int index = -1;
	return FindCardIndex(pCard, index);
}

BaseCard* Deck::GetNextCard(int add)
{
	auto index = currentCardIndex;

	for (int i = 0; add > 0; i++)
	{
		add--;
		index++;
		if (index > content.size() - 1) index = 0;
	}

	for(int i = 0; add < 0;i++)
	{
		add++;
		index--;
		if (index < 0) index = content.size() - 1;
	}

	return content[index];
}

BaseCard* Deck::GetCurrentCard()
{
	return content[currentCardIndex];
}

BaseCard* Deck::GetCardByID(int id)
{
	for (auto& card : content)
	{
		if (card == nullptr) continue;
		if (card->id == id) return card;
	}

	return nullptr;
}
#pragma endregion

#pragma region State/Save/Load

json::JSON* Deck::GetState()
{
	if (state == nullptr) state = new json::JSON;

	(*state)["currentCard"] = currentCardIndex;
	(*state)["deckContent"] = json::Array();
	for (auto& card : content)
	{
		if (card == nullptr) continue;
		(*state)["deckContent"].append(*card->GetState());
	}

	return state;
}

json::JSON Deck::GetSave()
{
	json::JSON save;

	save["currentCard"] = currentCardIndex;
	save["deckContent"] = json::Array();
	for (auto& card : content)
	{
		if (card == nullptr) continue;
		save["deckContent"].append(card->GetSave());
	}

	return save;
}

void Deck::SetSave(json::JSON save, Character* pCardOwner)
{
	ReturnContentToPool();

	currentCardIndex = (save)["currentCard"].ToInt();
	for (int i = 0;i < (save)["deckContent"].size();i++)
	{
		content.push_back(BaseCard::LoadSave(pCardOwner, (save)["deckContent"][i]));
	}
}

#pragma endregion