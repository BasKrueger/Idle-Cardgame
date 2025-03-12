#pragma once
#include "BaseEncounter.h"

class CombatEncounter : public BaseEncounter
{
private:
	bool IsOngoing() override;

	void InternalInitialize(std::unique_ptr<Player>* pPlayer) override;
	void InternalTick(std::unique_ptr<Player>* pPlayer) override;
	void InternalEnd(std::unique_ptr<Player>* pPlayer) override;
	void InternalReturnToPool() override;
};

