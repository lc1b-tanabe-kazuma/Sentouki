#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include "UI/Aim.h"
#include <memory>
#include "PlayerStateBase.h"

// 前方宣言
class Enemy;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* bulletModel, Aim* aim);
	void Update();
	void Draw();

	void Move();

	void Attack(Enemy* target);

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 半径を取得
	float GetRadius() const { return kRadius; }

	void OnCollision();

	// 無敵時間中かどうかを取得
	bool IsInvincible() const {
		if(pendingState_) {
			return pendingState_->IsInvincible();
		}

		return currentState_ && currentState_->IsInvincible();
	}

	~Player();

private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;

	// StateクラスからPlayerの状態変更を許可
	friend class NormalState;
	friend class InvincibleState;

	// 現在のState
	std::unique_ptr<PlayerStateBase> currentState_;

	// 次に切り替えるState
	std::unique_ptr<PlayerStateBase> pendingState_;

	// State切り替え
	void ChangeState(std::unique_ptr<PlayerStateBase> newState) {
		pendingState_ = std::move(newState);
	}

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

	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 10.0f;
};