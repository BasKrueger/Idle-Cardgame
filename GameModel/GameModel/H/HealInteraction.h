#pragma once
#include "H/BaseInteraction.h"

class HealInteraction : public BaseInteraction
{
public:
	int healing;
	void Initialize(int healing);

protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};


