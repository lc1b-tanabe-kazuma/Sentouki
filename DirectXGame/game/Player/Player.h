#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"

// 前方宣言
class Aim;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* bulletModel, Aim* aim);
	void Update();
	void Draw();

	void Move();

	void Attack();

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

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
	float kBulletSpeed = 1.0f;

	// 半径
	const float kRadius = 1.0f;

	//
	Aim* aim_ = nullptr;

	const float kAimZ = 22.5f;
};