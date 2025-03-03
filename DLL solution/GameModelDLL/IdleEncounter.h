#pragma once
#include "BaseEncounter.h"

class IdleEncounter : public BaseEncounter
{
private:
	bool IsOngoing() override;
	bool IsStaticEncounter() override;

	void InternalInitialize(std::unique_ptr<Character>* pPlayer) override;
	void InternalTick(std::unique_ptr<Character>* pPlayer) override;
	void InternalEnd(std::unique_ptr<Character>* pPlayer) override;
	void InternalReturnToPool() override;
};

