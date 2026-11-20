#pragma once

#include "TitleStateBase.h"

class StageSelectState : public TitleStateBase {
public:
	void Update(GameTitle* gameTitle) override;
	void Draw(GameTitle* gameTitle) override;
};