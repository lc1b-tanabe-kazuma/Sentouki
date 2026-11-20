#include "NormalState.h"
#include "InvincibleState.h"
#include "Player/Player.h"

void NormalState::Update(Player* player) {
	// 通常状態では特別な処理なし
	(void)player;
}

void NormalState::OnCollision(Player* player) {

	// 無敵状態へ切り替える
	player->ChangeState(
		std::make_unique<InvincibleState>()
	);
}