#include "Grand/Ground.h"
#include "MyMath.h"

using namespace KamataEngine;

// 初期化
void Ground::Initialize(Model* model, Camera* camera) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;

	worldTransform_.translation_.y = kHeight;
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