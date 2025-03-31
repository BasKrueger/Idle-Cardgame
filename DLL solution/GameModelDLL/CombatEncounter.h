#pragma once
#include "BaseEncounter.h"

class CombatEncounter : public BaseEncounter
{
public:
	static const int encounterID = 1;

private:
	bool IsOngoing() override;

	void InternalInitialize(std::unique_ptr<Player>* pPlayer, bool& hastStaticBackground, int& eID) override;
	void InternalTick(std::unique_ptr<Player>* pPlayer) override;
	void InternalEnd(std::unique_ptr<Player>* pPlayer) override;
};

