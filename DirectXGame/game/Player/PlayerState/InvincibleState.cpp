#include "InvincibleState.h"
#include "NormalState.h"
#include "Player/Player.h"

void InvincibleState::Update(Player* player) {

	// 30FPSで無敵時間を減らす
	timer_ -= 1.0f / 30.0f;

	// 無敵時間終了
	if(timer_ <= 0.0f) {

		player->ChangeState(
			std::make_unique<NormalState>()
		);
	}
}

bool InvincibleState::IsVisible() const {

	// 30フレーム周期で点滅
	return static_cast<int>(timer_ * 30.0f) % 10 >= 5;
}

void InvincibleState::OnCollision(Player* player) {
	// 無敵中なので何もしない
	(void)player;
}