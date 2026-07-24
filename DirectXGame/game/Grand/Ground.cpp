#include "Grand/Ground.h"
#include "MyMath.h"

// 初期化
void Ground::Initialize(Model* model, Camera* camera) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;

	worldTransform_.translation_ = { 0.0f, 1.0f, 0.0f };
	worldTransform_.scale_ = { 10.1f, 10.01f, 10.01f };
}

// 更新
void Ground::Update() {
	WorldTransformUpdate(worldTransform_);
}

// 描画
void Ground::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}