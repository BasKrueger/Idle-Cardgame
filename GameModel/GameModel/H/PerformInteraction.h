#pragma once
#include "BaseInteraction.h"

class PerformInteraction : public BaseInteraction
{
	BaseInteraction* pInteraction = 0;

public:
	void Initialize(BaseInteraction* interaction);
	void InternalPerform() override;
	void ReturnToPool() override;
};