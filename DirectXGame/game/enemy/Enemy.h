#pragma once
#include "KamataEngine.h"

class Enemy {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	//
	float GetRadius() { return kRadius; }

	// 敵の生成時の位置を設定
	void SetPosition(const KamataEngine::Vector3& position);

	// 敵の移動ベクルのセッター
	void SetMoveVector(const KamataEngine::Vector3& v) { moveVec_ = v; }

	// 画面外に出たかどうか
	bool IsOut() { return isOut_; }

	bool IsDead() { return isDead_; }

	// 画面外に出た時にデリートする
	void OutFlag();

	// 当たり判定時の処理
	void OnCollision() { isDead_ = true; }

private:

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTranseform_;

	// デフラグ
	bool isDead_ = false;

	// 画面外に出たかどうか
	bool isOut_ = false;

	// 半径
	const float kRadius = 2.5f;

	KamataEngine::Vector3 moveVec_ = { 0, 0, 0 };
};