#pragma once
#include <vector>

struct CardState;

class CharacterState
{
public:
	int id;
	int baseHP;
	int hp;
	int dmg;
	int phase;
	int animationTrigger;
	std::vector<CardState*> autoDeck;

};

