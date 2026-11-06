#pragma once

class StageClearCondition {
public:
	virtual ~StageClearCondition() = default;

	// クリア条件を満たしたか判定
	virtual bool IsClear() = 0;
};