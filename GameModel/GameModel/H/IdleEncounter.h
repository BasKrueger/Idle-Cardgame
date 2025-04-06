#pragma once
#include "BaseEncounter.h"

class IdleEncounter : public BaseEncounter
{
public:
	static const int encounterID = 0;

private:
	bool IsOngoing() override;

	void InternalInitialize(Player* pPlayer, bool& hastStaticBackground, int& eID) override;
	void InternalTick(Player* pPlayer) override;
	void InternalEnd(Player* pPlayer) override;
};

