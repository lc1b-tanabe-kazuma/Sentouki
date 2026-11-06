#include "StageBoss.h"

StageBoss::StageBoss(Boss* boss) : boss_(boss) { }

bool StageBoss::IsClear() {
	return boss_->IsDead();
}