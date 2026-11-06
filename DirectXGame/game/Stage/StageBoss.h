#pragma once
#include "StageClearCondition.h"
#include "boss/Boss.h"

class StageBoss : public StageClearCondition {
public:
	StageBoss(Boss* boss);

	bool IsClear() override;

private:
	Boss* boss_;
};