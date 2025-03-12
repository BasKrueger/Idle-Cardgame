#pragma once
#include "Character.h"
#include "DamageInteraction.h"
#include <memory>
#include "Json.hpp"
#include <vector>

class Player : public Character
{
public:
	void Before(DamageInteraction* interaction) override;
	void AddToCollection(BaseCard* card);

	int SwitchCards(int collection, int deck);
	json::JSON* GetState();

	int xp = 1;
	int gold = 1;

private:
	std::unique_ptr<json::JSON> state;
	std::vector<BaseCard*> collection;

protected:
	void InternalInitialize(int& baseHP, int& baseDamage, LocalizedString* characterName) override;
	void InternalReturnToPool() override;

};

