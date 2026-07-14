#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* bulletModel);
	void Update();
	void Draw();

	void Move();

	void Attack(KamataEngine::Vector2 aimPos);

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	~Player();

private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;

	// キーボード入力
	KamataEngine::Input* input_ = nullptr;

	float moveSpeed_ = 0.1f;

	// 弾丸
	std::list<PlayerBullet*> bullets_;

	KamataEngine::Model* bulletModel_ = nullptr;

	// 弾の速度
	KamataEngine::Vector3 kBulletVelocity = { 0.0f, 0.0f, 1.0f };

	// 半径
	const float kRadius = 1.0f;
};