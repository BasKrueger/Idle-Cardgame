#pragma once
#include "BaseInteraction.h"

class DieInteraction : public BaseInteraction
{
protected:
	void InternalPerform() override;
	void ReturnToPool() override;
};

