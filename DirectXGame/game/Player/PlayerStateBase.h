#pragma once

class Player;

class PlayerStateBase {
public:
	virtual ~PlayerStateBase() = default;

	// 状態ごとの更新
	virtual void Update(Player* player) = 0;

	// 無敵状態かどうか
	virtual bool IsInvincible() const {
		return false;
	}

	// プレイヤーを描画するか
	virtual bool IsVisible() const {
		return true;
	}

	// 被弾時の処理
	virtual void OnCollision(Player* player) = 0;
};