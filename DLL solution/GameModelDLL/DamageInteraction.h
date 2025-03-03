#pragma once
#include "BaseInteraction.h"

class DamageInteraction : public BaseInteraction
{
public:
	int attackDamage;

	void Initialize(int damage);

protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};


