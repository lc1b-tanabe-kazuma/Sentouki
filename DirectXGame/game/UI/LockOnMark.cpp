#include "LockOnMark.h"
#include "MYMath.h"
#include "enemy/Enemy.h"
#include <numbers>

using namespace KamataEngine;

void LockOnMark::Initialize(Model* model, Camera* camera, Enemy* target) {
	model_ = model;
	camera_ = camera;
	target_ = target;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.5f, 1.5f, 1.5f };

	camera_->Initialize();
}

void LockOnMark::Update() {

	if(target_ == nullptr) {
		return;
	}

	Vector3 enemyPos = target_->GetWorldPosition();

	// 敵からカメラへ向かう方向
	Vector3 toCamera =
		camera_->translation_ - enemyPos;

	toCamera = Normalize(toCamera);

	// カメラ側に少し出す
	worldTransform_.translation_ =
		enemyPos + toCamera * offset_;

	// ビルボード
	worldTransform_.rotation_.x =
		camera_->rotation_.x;

	worldTransform_.rotation_.y =
		camera_->rotation_.y;

	worldTransform_.rotation_.z =
		camera_->rotation_.z;

	WorldTransformUpdate(worldTransform_);
}

void LockOnMark::Draw() {
	model_->Draw(worldTransform_, *camera_);
}