#pragma once
#include "H/BaseInteraction.h"

class Buff;
class BaseCard;
class Character;

class AddBuffInteraction : public BaseInteraction
{
public:
	Buff* buff;
	BaseCard* target;

	void Initialize(Buff* buff, BaseCard* target);
	void Initialize(Buff* buff, Character* target);

protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};

