#pragma once
#include "BaseInteraction.h"

class BaseCard;

class PlayInteraction : public BaseInteraction
{
public:
	BaseCard* cardToPlay = nullptr;
	void Initialize(BaseCard* card);

protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};

