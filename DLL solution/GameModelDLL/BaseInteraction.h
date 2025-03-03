#pragma once
#include "Interactor.h"
#include <iostream>

class InterActor;

class BaseInteraction
{
public:
	InterActor* pSource;
	InterActor* pTarget;

	void BaseInitialization(InterActor* interactionSource, InterActor* interactionTarget);
	void Perform();

public:
	virtual void InternalPerform();
	virtual void ReturnToPool();
};