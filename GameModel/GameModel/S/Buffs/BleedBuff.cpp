#include "H/BleedBuff.h"
#include "H/InteractionPool.h"
#include "H/InteractionManager.h"

void BleedBuff::InternalInitialize(int& id, int& duration, std::array<int, 3>& variables, std::string& iconName)
{
	id = buffID;
	variables[0] = 1;

	duration = 4;
	iconName = "undefined";
}

void BleedBuff::OnTick()
{
	auto dmg = InteractionPool<DamageInteraction>().GetInstance(pOwner, pOwner);
	dmg->Initialize(variables[0]);
	InteractionManager::AddNext(dmg);
}
