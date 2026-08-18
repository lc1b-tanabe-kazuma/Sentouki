#pragma once
#include "KamataEngine.h"

// 前方宣言
class Enemy;

class PlayerBullet {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, KamataEngine::Vector3& velocity);

	void Update();

	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() { return isDead_; }

	void OnCollision() { isDead_ = true; }

	// プレイヤーの弾の位置を取得
	KamataEngine::Vector3 GetPosition();

	// プレイヤーの弾の半径
	const float& GetRadius() const { return kRadius; }

	// スケール設定
	void setScale(const KamataEngine::Vector3& scale) { worldTranseform_.scale_ = scale; }

	// 目標の敵を設定
	void SetTarget(Enemy* target) {
		target_ = target;
	}

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTranseform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// 速度
	KamataEngine::Vector3 velocity_;

	// 弾の寿命
	static const int32_t kBulletLife = 60 * 3; // 3秒

	// デスタイマー
	int32_t deathTimer_ = kBulletLife;

	// デスフラグ
	bool isDead_ = false;

	// 半径
	const float kRadius = 1.0f;

	// 目標の敵
	Enemy* target_ = nullptr;

	// 弾の速度
	const float kBulletSpeed = 0.5f;
};