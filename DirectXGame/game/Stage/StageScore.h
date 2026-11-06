#pragma once
#include "StageClearCondition.h"

class StageScore : public StageClearCondition {
public:
    StageScore(int* score);
    bool IsClear() override;

private:
    int* score_;
};