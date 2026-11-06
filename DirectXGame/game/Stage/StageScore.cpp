#include "StageScore.h"

StageScore::StageScore(int* score)
	: score_(score) { }

bool StageScore::IsClear() {
	return score_ != nullptr && *score_ >= 2000;
}