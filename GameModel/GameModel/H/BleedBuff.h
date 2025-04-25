#pragma once
#include "H/Buff.h"

class BleedBuff : public Buff
{
public:
	static const int buffID = 0;

protected:
	virtual void InternalInitialize(int& id, int& duration, std::array<int, 3>& variables, std::string& iconName) override;
	virtual void OnTick() override;
};

