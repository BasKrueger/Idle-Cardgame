#pragma once
#include "BaseEncounter.h"

class CombatEncounter : public BaseEncounter
{
private:
	bool IsOngoing() override;

	void InternalInitialize(std::unique_ptr<Character>* Character) override;
	void InternalTick(std::unique_ptr<Character>* Character) override;
	void InternalEnd(std::unique_ptr<Character>* Character) override;
	void InternalReturnToPool() override;
};

