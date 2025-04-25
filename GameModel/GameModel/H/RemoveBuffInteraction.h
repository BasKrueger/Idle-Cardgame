#pragma once
#include "H/BaseInteraction.h"

class Buff;
class BaseCard;

class RemoveBuffInteraction : public BaseInteraction
{
public:
	Buff* buff;
	BaseCard* target;

	void Initialize(Buff* buff, BaseCard* target);

protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};
