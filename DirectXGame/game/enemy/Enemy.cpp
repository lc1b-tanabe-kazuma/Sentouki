#define NOMINMAX
#include "Enemy.h"
#include "MyMath.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera) {

	// NULLポインタのチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTranseform_.Initialize();

	worldTranseform_.scale_ = { 0.5f, 0.5f, 0.5f };

	// 引数の内容をメンバ変数に記録
	camera_ = camera;
}

void Enemy::Update() {
	if(isDead_ || isOut_) {
		return;
	}

	// 移動
	worldTranseform_.translation_ += moveVec_;

	Vector3 dir = Normalize(moveVec_);

	worldTranseform_.rotation_.y = std::atan2(dir.x, dir.z);

	// 行列を定数バッファに転送
	worldTranseform_.TransferMatrix();
	WorldTransformUpdate(worldTranseform_);
}

Vector3 Enemy::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTranseform_.matWorld_.m[3][0];
	worldPos.y = worldTranseform_.matWorld_.m[3][1];
	worldPos.z = worldTranseform_.matWorld_.m[3][2];
	return worldPos;
}

// 敵の生成時の位置を設定
void Enemy::SetPosition(const Vector3& position) { worldTranseform_.translation_ = position; }

void Enemy::Draw() {

	if(!isDead_ && !isOut_) {
		// 敵の描画
		model_->Draw(worldTranseform_, *camera_);
	}
}

// 画面外に出た時にデリートする
void Enemy::OutFlag() {
	if(worldTranseform_.translation_.z <= -5.5f || worldTranseform_.translation_.x <= -40.0f || worldTranseform_.translation_.x >= 40.0f) {
		isOut_ = true;
	}
}