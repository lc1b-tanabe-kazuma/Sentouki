#pragma once
#include "Player/PlayerStateBase.h"

class InvincibleState : public PlayerStateBase {
public:
	void Update(Player* player) override;

	bool IsInvincible() const override {
		return true;
	}

	bool IsVisible() const override;

	void OnCollision(Player* player) override;

private:
	float timer_ = 2.0f;

	const float kInvincibleTime_ = 2.0f;
};